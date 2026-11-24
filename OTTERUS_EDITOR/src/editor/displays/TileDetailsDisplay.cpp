#include "TileDetailsDisplay.h"
#include "editor/utilities/ImGuiUtils.h"
#include "editor/utilities/DrawComponentUtils.h"
#include "editor/scene/SceneManager.h"
#include "editor/scene/SceneObject.h"
#include "editor/tools/ToolManager.h"
#include "editor/tools/TileTool.h"

#include "Core/ECS/MainRegistry.h"
#include "Core/ECS/Components/AllComponents.h"
#include "Core/Resources/AssetManager.h"
#include "Core/CoreUtilities/CoreUtilities.h"
#include "OtterusUtilities/HelperUtilities.h"
#include "Logger/Logger.h"

#include <imgui.h>
#include <ranges>



using namespace otterus_core::ECS;

namespace otterus_editor
{
    void TileDetailsDisplay::DrawSpriteComponent(SpriteComponent& sprite, SceneObject* scene)
    {
        bool changed{ false };

        ImGui::SeparatorText( "Sprite" );
        ImGui::PushID( entt::type_hash<SpriteComponent>::value() );
        if ( ImGui::TreeNodeEx( "##SpriteTree", ImGuiTreeNodeFlags_DefaultOpen ) )
        {
            ImGui::AddSpaces( 2 );

            ImGui::InlineLabel( "texture: " );
            ImGui::TextColored( ImVec4{ 0.f, 1.f, 0.f, 1.f }, sprite.texture_name.c_str() );

            std::string layer{ "" };

            // TODO: Add layer description
            if ( sprite.layer >= 0 && sprite.layer < scene->GetLayerParams().size() )
            {
                layer = scene->GetLayerParams()[ sprite.layer ].layerName;
            }

            ImGui::InlineLabel( "layer: " );
            ImGui::TextColored( ImVec4{ 0.f, 1.f, 0.f, 1.f }, layer.c_str() );

            ImGui::AddSpaces( 2 );

            // Color picker
            ImVec4 col = { sprite.color.r / 255.f, sprite.color.g / 255.f, sprite.color.b / 255.f, sprite.color.a / 255.f };
            ImGui::InlineLabel( "color" );
            if ( ImGui::ColorEdit4( "##color", &col.x, IMGUI_COLOR_PICKER_FLAGS ) )
            {
                sprite.color.r = static_cast<GLubyte>( col.x * 255.f );
                sprite.color.g = static_cast<GLubyte>( col.y * 255.f );
                sprite.color.b = static_cast<GLubyte>( col.z * 255.f );
                sprite.color.a = static_cast<GLubyte>( col.w * 255.f );
            }

            ImGui::AddSpaces( 2 );

            ImGui::PushItemWidth( 120.f );
            ImGui::InlineLabel( "width" );
            if ( ImGui::InputFloat( "##width", &sprite.width, 8.f, 8.f ) )
            {
                sprite.width = std::clamp( sprite.width, 8.f, 1366.f );
                changed = true;
            }

            ImGui::InlineLabel( "height" );
            if ( ImGui::InputFloat( "##height", &sprite.height, 8.f, 8.f ) )
            {
                sprite.height = std::clamp( sprite.height, 8.f, 768.f );
                changed = true;
            }

            ImGui::TreePop();
            ImGui::PopItemWidth();
        }

        ImGui::PopID();

        if ( changed )
        {
            auto texture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name );
            if ( !texture )
                return;

            otterus_core::GenerateUVs( sprite, texture->GetWidth(), texture->GetHeight() );
        }
    }

    TileDetailsDisplay::TileDetailsDisplay()
        : m_SelectedLayer{-1}
        , m_RenameLayerBuf{ "" }
        , m_Rename{false}
    {}

    TileDetailsDisplay::~TileDetailsDisplay()
    {}

    void TileDetailsDisplay::Draw()
    {
        auto currentScene = SCENE_MANAGER().GetCurrentScene();
        auto& toolManager = SCENE_MANAGER().GetToolManager();

        auto activeTool = toolManager.GetActiveTool();
        if ( !activeTool )
            return;

        if ( !ImGui::Begin( "Tile Details" ) || !currentScene )
        {
            ImGui::End();
            return;
        }

        auto& tileData = activeTool->GetTileData();

	// Transform
	DrawComponentUtils::DrawComponentInfo( tileData.transform );

	ImGui::AddSpaces( 2 );
	// Sprite
	DrawSpriteComponent( tileData.sprite, currentScene.get() );

	ImGui::AddSpaces( 2 );
	ImGui::Separator();
	ImGui::AddSpaces( 2 );

	ImGui::Checkbox( "Box Collider", &tileData.hasCollider );
	if ( tileData.hasCollider )
	{
		DrawComponentUtils::DrawComponentInfo( tileData.boxCollider );
	}

	ImGui::AddSpaces( 2 );
	ImGui::Separator();
	ImGui::AddSpaces( 2 );

	ImGui::Checkbox( "Cicle Collider", &tileData.isCircle );
	if ( tileData.isCircle )
	{
		DrawComponentUtils::DrawComponentInfo( tileData.circleCollider );
	}

	ImGui::AddSpaces( 2 );
	ImGui::Separator();
	ImGui::AddSpaces( 2 );

	ImGui::Checkbox( "Animation", &tileData.hasAnimation );
	if ( tileData.hasAnimation )
	{
		DrawComponentUtils::DrawComponentInfo( tileData.animation );
	}

	ImGui::AddSpaces( 2 );
	ImGui::Separator();
	ImGui::AddSpaces( 2 );

	ImGui::Checkbox( "Physics", &tileData.enablePhysics );
	if ( tileData.enablePhysics )
	{
		DrawComponentUtils::DrawComponentInfo( tileData.physics );
	}

	ImGui::AddSpaces( 2 );
	ImGui::Separator();
	ImGui::AddSpaces( 2 );

	if ( ImGui::BeginChild( " Tile Layers" ) )
	{
		ImGui::SeparatorText( "Tile Layers" );
		auto& spriteLayers = currentScene->GetLayerParams();

		if ( ImGui::Button( "Add" ) )
		{
			currentScene->AddNewLayer();
		}

		ImGui::AddSpaces( 2 );
		ImGui::Separator();
		ImGui::AddSpaces( 2 );

		float itemWidth{ ImGui::GetWindowWidth() - 32.f };
		auto rView = spriteLayers | std::ranges::views::reverse;

		for ( auto rit = rView.begin(); rit != rView.end(); rit++ )
		{
			int n = std::distance( rit, rView.end() ) - 1;

			auto& spriteLayer = *rit;
			bool bIsSelected{ m_SelectedLayer == n };

			ImGui::Selectable( spriteLayer.layerName.c_str(), bIsSelected, 0, ImVec2{ itemWidth, 20.f } );

			if ( ImGui::IsItemActive() && !ImGui::IsItemHovered() )
			{
				int n_next = n + ( ImGui::GetMouseDragDelta( 0 ).y < 0.f ? 1 : -1 );
				if ( n_next >= 0 && n_next < rView.size() )
				{
					// We need to swap the sprite layers here, not the reverse view.
					// Because the indexes are not reversed anymore.
					std::swap( spriteLayers[ n ], spriteLayers[ n_next ] );

					auto spriteView = currentScene->GetRegistry().GetRegistry().view<SpriteComponent, TileComponent>();

					for ( auto entity : spriteView )
					{
						auto& sprite = spriteView.get<SpriteComponent>( entity );
						if ( sprite.layer == n )
						{
							sprite.layer = n_next;
						}
						else if ( sprite.layer == n_next )
						{
							sprite.layer = n;
						}
					}

					m_SelectedLayer = n_next;
					tileData.sprite.layer = n_next;

					ImGui::ResetMouseDragDelta();
				}
			}

			// Set the current selected layer
			if ( ImGui::IsItemHovered() && ImGui::IsMouseClicked( 0 ) && !m_Rename )
			{
				m_SelectedLayer = n;
				tileData.sprite.layer = n;
			}

			// Start the rename
			if ( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( 0 ) && !m_Rename && bIsSelected )
			{
				m_Rename = true;
				m_RenameLayerBuf.clear();
				m_RenameLayerBuf = spriteLayer.layerName;
			}

			if ( m_Rename && bIsSelected )
			{
				// TODO: Rename stuff
			}

			ImGui::SameLine();

			// Layer Visibility
			ImGui::Checkbox( fmt::format( "##visible_{}", spriteLayer.layerName ).c_str(), &spriteLayer.visible );
		}

		ImGui::EndChild();
	}

	ImGui::End();
    }
}
