#include "TileTool.h"
#include "Logger/Logger.h"
#include "editor/utilities/EditorUtilities.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/Resources/AssetManager.h"
#include "Rendering/Core/BatchRenderer.h"
#include "Rendering/Core/Camera2D.h"

constexpr int MOUSE_SPRITE_LAYER = 10;

using namespace otterus_core::ECS;

namespace otterus_editor
{
    uint32_t TileTool::CheckForTile(const glm::vec2& position)
    {
        if ( !m_Registry )
            return entt::null;

        auto tileView = m_Registry->GetRegistry().view<TileComponent, TransformComponent>();

        for ( auto entity : tileView )
        {
            Entity tile{ *m_Registry, entity };
            const auto& transform = tile.GetComponent<TransformComponent>();
            const auto& sprite = tile.GetComponent<SpriteComponent>();

            if ( position.x >= transform.position.x &&
                 position.x < transform.position.x + sprite.width * transform.scale.x &&
                 position.y >= transform.position.y &&
                 position.y < transform.position.y + sprite.height * transform.scale.y &&
                 m_MouseTile->sprite.layer == sprite.layer )
            {
                return static_cast<uint32_t>( entity );
            }
        }

        return entt::null;
    }

    Entity TileTool::CreateEntity()
    {
        OTTERUS_ASSERT( m_Registry && "The registry must be valid to create an entity." );
        return Entity{ *m_Registry, "", "" };
    }

    Entity TileTool::CreateEntity(uint32_t id)
    {
        OTTERUS_ASSERT( m_Registry && "The registry must be valid to create an entity." );
        return Entity{ *m_Registry, static_cast<entt::entity>( id ) };
    }

    // NOTE: Uses Batch Renderer which is
    //       overkill for a single sprite,
    //       subjected to change later.
    void TileTool::DrawMouseSprite()
    {
        m_BatchRenderer->Begin();
        const auto& sprite = m_MouseTile->sprite;
        const auto& transform = m_MouseTile->transform;

        glm::vec4 position{ transform.position.x,
                            transform.position.y,
                            sprite.width * transform.scale.x,
                            sprite.height * transform.scale.y };

        glm::vec4 uvs{ sprite.uvs.u, sprite.uvs.v, sprite.uvs.uv_width, sprite.uvs.uv_height };

        const auto texture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name );
        if ( texture )
            m_BatchRenderer->AddSprite(
                position, uvs, texture->GetID(), MOUSE_SPRITE_LAYER, glm::mat4{ 1.f }, sprite.color );

        m_BatchRenderer->End();
        m_BatchRenderer->Render();
    }

    void TileTool::ExamineMousePosition()
    {
        if ( !m_Camera )
            return;

        const auto& mouseWorldPos = GetMouseWorldCoords();
        auto& transform = m_MouseTile->transform;


        bool isOffset{ true };

        if ( m_GridSnap )
        {

            float tileWidth  = m_MouseRect.x * transform.scale.x;
            float tileHeight = m_MouseRect.y * transform.scale.y;
            glm::vec2 mouseGrid{
                mouseWorldPos.x / tileWidth,
                mouseWorldPos.y / tileHeight
            };
            transform.position.x = std::floor( mouseGrid.x ) * tileWidth;
            transform.position.y = std::floor( mouseGrid.y ) * tileHeight;
            m_GridCoords.x = std::floor( mouseGrid.x );
            m_GridCoords.y = std::floor( mouseGrid.y );
            SetMouseWorldCoords( transform.position );

        }
        else
        {
            transform.position = isOffset ? mouseWorldPos - m_MouseRect * 0.5f : mouseWorldPos;
        }
    }

    TileTool::TileTool()
    : AbstractTool()
    , m_MouseRect{ 16.f }
    , m_GridCoords{ 0.f }
    , m_GridSnap{ true }
    , m_BatchRenderer{ std::make_shared<otterus_rendering::SpriteBatchRenderer>() }
    , m_MouseTile{ std::make_shared<Tile>() }
    {}

    void TileTool::Update(Canvas& canvas)
    {
        AbstractTool::Update(canvas);
        ExamineMousePosition();
    }

    void TileTool::ClearMouseTextureData()
    {
        m_MouseTile->sprite = SpriteComponent{};
    }

    void TileTool::LoadSpriteTextureData(const std::string& textureName)
    {
        m_MouseTile->sprite = SpriteComponent{
            .width = m_MouseRect.x,
            .height = m_MouseRect.y,
            .color = otterus_rendering::Color{ 255, 255, 255, 255 },
            .start_x = 0,
            .start_y = 0,
            .texture_name = textureName,
            .layer = 0,
        };

        auto texture = MAIN_REGISTRY().GetAssetManager().GetTexture( textureName );
        OTTERUS_ASSERT( texture && "Texture must exist" );
        m_MouseTile->sprite.generate_uvs( texture->GetWidth(), texture->GetHeight() );
    }

    const std::string& TileTool::GetSpriteTexture() const
    {
        return m_MouseTile->sprite.texture_name;
    }

    void TileTool::SetBoxCollider(int width, int height, const glm::vec2& offset)
    {
        m_MouseTile->boxCollider.width = width;
        m_MouseTile->boxCollider.height = height;
        m_MouseTile->boxCollider.offset = offset;
    }

    void TileTool::SetSpriteLayer(int layer)
    {
        m_MouseTile->sprite.layer = layer;
    }

    void TileTool::SetScale(const glm::vec2& scale)
    {
        m_MouseTile->transform.scale = scale;
    }

    void TileTool::SetAnimation(int numFrames, int frameRate, bool isVertical, bool looped, int frameOffset)
    {
        m_MouseTile->animation.numFrames = numFrames;
        m_MouseTile->animation.frameRate = frameRate;
        m_MouseTile->animation.vertical = isVertical;
        m_MouseTile->animation.looped = looped;
        m_MouseTile->animation.frameOffset = frameOffset;
    }

    void TileTool::SetSpriteUVs(int startX, int startY)
    {
        m_MouseTile->sprite.start_x = startX;
        m_MouseTile->sprite.start_y = startY;
        m_MouseTile->sprite.uvs.u = startX * m_MouseTile->sprite.uvs.uv_width;
        m_MouseTile->sprite.uvs.v = startY * m_MouseTile->sprite.uvs.uv_height;
    }

    void TileTool::SetSpriteRect(glm::vec2& spriteRect)
    {
        if ( m_MouseTile->sprite.texture_name.empty() )
            return;

        m_MouseRect = spriteRect;
        auto& sprite = m_MouseTile->sprite;
        sprite.width = m_MouseRect.x;
        sprite.height = m_MouseRect.y;

        auto texture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name );
        OTTERUS_ASSERT( texture && "Texture Must exist." );
        sprite.generate_uvs( texture->GetWidth(), texture->GetHeight() );
    }

    void TileTool::SetCollider(bool collider)
    {
        m_MouseTile->hasCollider = collider;
    }

    void TileTool::SetCircle(bool circle)
    {
        m_MouseTile->isCircle = circle;
    }

    void TileTool::SetAnimation(bool animated)
    {
        m_MouseTile->hasAnimation = animated;
    }

    void TileTool::SetPhysics(bool physics)
    {
        m_MouseTile->enablePhysics = physics;
    }

    const bool TileTool::SpriteValid() const
    {
        return !m_MouseTile->sprite.texture_name.empty();
    }

    const bool TileTool::CanDrawOrCreate() const
    {
        return IsActivated() && !OutOfBounds() && IsOverTilemapWindow() && SpriteValid();
    }
}
