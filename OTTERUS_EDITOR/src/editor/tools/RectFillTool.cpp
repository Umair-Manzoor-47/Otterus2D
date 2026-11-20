#include "RectFillTool.h"
#include "Core/Resources/AssetManager.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/ECS/Components/TransformComponent.h"
#include "Core/ECS/Components/SpriteComponent.h"
#include "Core/ECS/Components/TileComponent.h"
#include "Rendering/Core/Camera2D.h"
#include "Rendering/Core/RectBatchRenderer.h"
#include "Rendering/Essentials/Primitives.h"
#include "Rendering/Essentials/Vertex.h"
#include "Logger/Logger.h"
#include "editor/utilities/EditorUtilities.h"
#include "Rendering/Core/BatchRenderer.h"

using namespace otterus_rendering;
using namespace otterus_core::ECS;

constexpr int PREVIEW_LAYER = 10;

namespace otterus_editor
{
	void RectFillTool::CreateTiles()
	{
		const auto& sprite = m_MouseTile->sprite;
		const auto& transform = m_MouseTile->transform;
		auto texture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name );
		if ( !texture )
			return;

		const int dx = static_cast<int>( m_TileFillRect->width );
		const int dy = static_cast<int>( m_TileFillRect->height );

		auto spriteWidth = static_cast<int>( sprite.width * transform.scale.x * ( dx > 0 ? 1.f : -1.f ) );
		auto spriteHeight = static_cast<int>( sprite.height * transform.scale.y * ( dy > 0 ? 1.f : -1.f ) );

		for ( int y = 0; ( dy > 0 ? y < dy : y > dy ); y += spriteHeight )
		{
			for ( int x = 0; ( dx > 0 ? x < dx : x > dx ); x += spriteWidth )
			{
				glm::vec2 newTilePosition{ m_StartPressPos.x + x, m_StartPressPos.y + y };

				// Check if there is already a tile
				if ( auto id = CheckForTile( newTilePosition ); id != entt::null )
					continue;

				Entity tile{ CreateEntity() };

				auto& newTransform = tile.AddComponent<TransformComponent>( transform );
				newTransform.position = newTilePosition;

				tile.AddComponent<SpriteComponent>( sprite );

				if ( m_MouseTile->hasCollider )
				{
					// TODO: add collider component
				}

				if ( m_MouseTile->hasAnimation )
				{
					// TODO: add animation component
				}

				if ( m_MouseTile->enablePhysics )
				{
					// TODO: add physics component
				}

				tile.AddComponent<TileComponent>( static_cast<uint32_t>( tile.GetEntity() ) );
			}
		}
	}

	void RectFillTool::RemoveTiles()
	{
		const auto& sprite = m_MouseTile->sprite;
		const auto& transform = m_MouseTile->transform;
		const int dx = static_cast<int>( m_TileFillRect->width );
		const int dy = static_cast<int>( m_TileFillRect->height );

		auto spriteWidth = static_cast<int>( sprite.width * transform.scale.x * ( dx > 0 ? 1.f : -1.f ) );
		auto spriteHeight = static_cast<int>( sprite.height * transform.scale.y * ( dy > 0 ? 1.f : -1.f ) );

		std::set<std::uint32_t> entitiesToRemove{};

		for ( int y = 0; ( dy > 0 ? y < dy : y > dy ); y += spriteHeight )
		{
			for ( int x = 0; ( dx > 0 ? x < dx : x > dx ); x += spriteWidth )
			{
				if (auto id = CheckForTile(glm::vec2{ m_StartPressPos.x + x, m_StartPressPos.y + y }); id != entt::null)
				{
					entitiesToRemove.insert( id );
				}
			}
		}

		for (auto id : entitiesToRemove)
		{
			Entity removedTile{ CreateEntity( id ) };
			removedTile.Kill();
		}
	}

	void RectFillTool::DrawPreview( int dx, int dy )
	{
		const auto& sprite = m_MouseTile->sprite;
		const auto& transform = m_MouseTile->transform;
		auto texture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name );
		if ( !texture )
			return;

		auto spriteWidth = static_cast<int>( sprite.width * transform.scale.x * ( dx > 0 ? 1.f : -1.f ) );
		auto spriteHeight = static_cast<int>( sprite.height * transform.scale.y * ( dy > 0 ? 1.f : -1.f ) );

		for ( int y = 0; ( dy > 0 ? y < dy : y > dy ); y += spriteHeight )
		{
			for ( int x = 0; ( dx > 0 ? x < dx : x > dx ); x += spriteWidth )
			{
				glm::vec4 tilePosition{ m_StartPressPos.x + x,
										m_StartPressPos.y + y,
										sprite.width * transform.scale.x,
										sprite.height * transform.scale.y };

				glm::vec4 uvs{ sprite.uvs.u, sprite.uvs.v, sprite.uvs.uv_width, sprite.uvs.uv_height };

				m_BatchRenderer->AddSprite(
					tilePosition, uvs, texture->GetID(), PREVIEW_LAYER, glm::mat4{ 1.f }, sprite.color );
			}
		}
	}

	void RectFillTool::ResetTile()
	{
		*m_TileFillRect = Rect{};
	}

	RectFillTool::RectFillTool()
		: TileTool()
		, m_ShapeRenderer{ std::make_unique<RectBatchRenderer>() }
		, m_TileFillRect{ std::make_unique<Rect>() }
		, m_StartPressPos{ 0.f }
	{
	}

	RectFillTool::~RectFillTool()
	{}

	void RectFillTool::Create()
	{
		if ( !CanDrawOrCreate() )
			return;

		if ( MouseBtnJustPressed( EMouseButton::LEFT ) || MouseBtnJustPressed( EMouseButton::RIGHT ) )
			m_StartPressPos = GetMouseWorldCoords();

		if ( MouseBtnJustReleased( EMouseButton::LEFT ) )
		{
			CreateTiles();
			ResetTile();
		}
		else if ( MouseBtnJustReleased( EMouseButton::RIGHT ) )
		{
			RemoveTiles();
			ResetTile();
		}
	}
	void RectFillTool::Draw()
	{
		if ( !CanDrawOrCreate() )
			return;

		auto shader = MAIN_REGISTRY().GetAssetManager().GetShader( "basic" );
		auto colorShader = MAIN_REGISTRY().GetAssetManager().GetShader( "color" );
		if ( !shader || !colorShader )
			return;

		shader->Enable();
		auto camMat = m_Camera->GetCameraMatrix();
		shader->SetUniformMat4( "uProjection", camMat );
		DrawMouseSprite();

		bool leftMousePressed{ MouseBtnPressed( EMouseButton::LEFT ) };
		bool rightMousePressed{ MouseBtnPressed( EMouseButton::RIGHT ) };
		if ( !leftMousePressed && !rightMousePressed )
			return;

		const auto& mouseWorldCoords = GetMouseWorldCoords();
		auto dx = mouseWorldCoords.x - m_StartPressPos.x;
		auto dy = mouseWorldCoords.y - m_StartPressPos.y;

		const auto& sprite = m_MouseTile->sprite;
		const auto& transform = m_MouseTile->transform;

		auto spriteWidth = sprite.width * transform.scale.x;
		auto spriteHeight = sprite.height * transform.scale.y;

		dx += dx > 0 ? spriteWidth : -spriteWidth;
		dy += dy > 0 ? spriteHeight : -spriteHeight;

		auto newPosX = m_StartPressPos.x + ( dx > 0 ? 0 : spriteWidth );
		auto newPosY = m_StartPressPos.y + ( dy > 0 ? 0 : spriteHeight );

		Color color{};

		if ( leftMousePressed )
		{
			color = { 0x7D, 0xF9, 0xFF, 100 };
			m_BatchRenderer->Begin();
			DrawPreview( dx, dy );
			m_BatchRenderer->End();
			m_BatchRenderer->Render();
			shader->Disable();
		}
		else if ( rightMousePressed )
		{
			color = { 0xFF, 0x73, 0x77, 100 };
		}

		colorShader->Enable();
		m_ShapeRenderer->Begin();
		m_ShapeRenderer->AddRect( Rect{ .position = {newPosX, newPosY}, .width = dx, .height = dy, .color = color } );
		m_ShapeRenderer->End();
		m_ShapeRenderer->Render();
		colorShader->Disable();

		m_TileFillRect->position = { newPosX, newPosY };
		m_TileFillRect->width = dx;
		m_TileFillRect->height = dy;
	}
}