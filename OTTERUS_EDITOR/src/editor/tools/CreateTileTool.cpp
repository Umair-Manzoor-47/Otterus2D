#include "CreateTileTool.h"
#include "Core/Resources/AssetManager.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/ECS/Components/TransformComponent.h"
#include "Core/ECS/Components/SpriteComponent.h"
#include "Core/ECS/Components/TileComponent.h"
#include "Rendering/Core/Camera2D.h"
#include "Logger/Logger.h"
#include "editor/utilities/EditorUtilities.h"

namespace otterus_editor{

    void CreateTileTool::CreateNewTile()
    {
        const auto& mouseWorldCoords = GetMouseWorldCoords();

        if ( auto id = CheckForTile( mouseWorldCoords ); id != entt::null )
            return;

        otterus_core::ECS::Entity tile{ CreateEntity() };

        tile.AddComponent<otterus_core::ECS::TransformComponent>( m_MouseTile->transform );
        tile.AddComponent<otterus_core::ECS::SpriteComponent>( m_MouseTile->sprite );
        if ( m_MouseTile->hasCollider )
        {
            // TODO: Add collider component when implemented
        }
        if ( m_MouseTile->hasAnimation )
        {
            // TODO: Add animation component when implemented
        }
        if ( m_MouseTile->enablePhysics )
        {
            // TODO: Add physics component when implemented
        }

        tile.AddComponent<otterus_core::ECS::TileComponent>( static_cast<uint32_t>( tile.GetEntity() ) );
    }

    void CreateTileTool::RemoveTile()
    {
        const auto& mouseWorldCoords = GetMouseWorldCoords();

        if ( auto id = CheckForTile( mouseWorldCoords ); id != entt::null )
        {
            otterus_core::ECS::Entity tileToRemove{ CreateEntity( id ) };
            tileToRemove.Kill();
        }
    }

    CreateTileTool::CreateTileTool()
    : TileTool()
    {}

    void CreateTileTool::Create() {

        if ( OutOfBounds() || !IsActivated() || !SpriteValid() || !m_Registry )
            return;

        if ( MouseBtnJustPressed( EMouseButton::LEFT ) || MouseBtnPressed( EMouseButton::LEFT ) )
        {
            CreateNewTile();
        }

        else if ( MouseBtnJustPressed( EMouseButton::RIGHT ) || MouseBtnPressed( EMouseButton::RIGHT ) )
        {
            RemoveTile();
        }
    }

    void CreateTileTool::Draw() {
        if ( !SpriteValid() || !IsActivated() || OutOfBounds() || !m_Camera  )
            return;

        auto shader = MAIN_REGISTRY().GetAssetManager().GetShader( "basic" );
        if ( !shader )
            return;

        shader->Enable();
        auto camMat = m_Camera->GetCameraMatrix();
        shader->SetUniformMat4( "uProjection", camMat);
        DrawMouseSprite();
        shader->Disable();
    }
}
