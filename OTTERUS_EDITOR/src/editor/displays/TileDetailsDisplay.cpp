#include "TileDetailsDisplay.h"
#include "editor/utilities/ImGuiUtils.h"
#include "editor/utilities/EditorTheme.h"
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
#include <algorithm>
#include <ranges>

using namespace otterus_core::ECS;

namespace otterus_editor
{
    void TileDetailsDisplay::DrawSpriteComponent( SpriteComponent& sprite, SceneObject* scene )
    {
        bool changed{ false };

        ImGui::PushID( entt::type_hash<SpriteComponent>::value() );
        if ( ui::ComponentSectionHeader( "Sprite" ) )
        {
            if ( ui::BeginPropertyTable( "##TileSpriteProps" ) )
            {
                const auto& highlightText = EditorTheme::GetTokens().highlightText;
                ui::PropertyLabel( "Texture" );
                ImGui::TextColored( highlightText, "%s", sprite.texture_name.c_str() );

                std::string layer{ "" };
                if ( sprite.layer >= 0 && sprite.layer < scene->GetLayerParams().size() )
                {
                    layer = scene->GetLayerParams()[ sprite.layer ].layerName;
                }

                ui::PropertyLabel( "Layer" );
                ImGui::TextColored( highlightText, "%s", layer.c_str() );

                ui::PropertyLabel( "Color" );
                ui::ColorEdit4U8( "##color", sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a );

                ui::PropertyLabel( "Width" );
                changed |= ImGui::DragFloat( "##width", &sprite.width, 1.0f, 8.f, 1366.f, "%.1f", ImGuiSliderFlags_AlwaysClamp );

                ui::PropertyLabel( "Height" );
                changed |= ImGui::DragFloat( "##height", &sprite.height, 1.0f, 8.f, 768.f, "%.1f", ImGuiSliderFlags_AlwaysClamp );

                ui::EndPropertyTable();
            }
        }

        ImGui::PopID();

        if ( changed )
        {
            if ( auto texture = MAIN_REGISTRY().GetAssetManager().GetTexture( sprite.texture_name ) )
            {
                otterus_core::GenerateUVs( sprite, texture->GetWidth(), texture->GetHeight() );
            }
        }
    }

    TileDetailsDisplay::TileDetailsDisplay()
        : m_SelectedLayer{ -1 }
        , m_RenameLayerBuf{ "" }
        , m_Rename{ false }
    {}

    TileDetailsDisplay::~TileDetailsDisplay()
    {}

    void TileDetailsDisplay::Draw()
    {
        if ( !ImGui::Begin( "Tile Details" ) )
        {
            ImGui::End();
            return;
        }

        auto currentScene = SCENE_MANAGER().GetCurrentScene();
        auto activeTool = SCENE_MANAGER().GetToolManager().GetActiveTool();
        if ( !currentScene || !activeTool )
        {
            ImGui::TextDisabled( "Select a scene and an active tile tool to inspect tile details." );
            ImGui::End();
            return;
        }

        auto& tileData = activeTool->GetTileData();

        // Transform
        DrawComponentUtils::DrawComponentInfo( tileData.transform );
        ImGui::Spacing();

        // Sprite
        DrawSpriteComponent( tileData.sprite, currentScene.get() );

        auto drawSectionToggle = []( const char* label, bool& enabled, auto&& drawBody ) {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ui::ToggleSwitch( label, &enabled );
            if ( enabled )
            {
                drawBody();
            }
        };

        drawSectionToggle( "Box Collider", tileData.hasCollider, [&]() {
            DrawComponentUtils::DrawComponentInfo( tileData.boxCollider );
        } );
        drawSectionToggle( "Circle Collider", tileData.isCircle, [&]() {
            DrawComponentUtils::DrawComponentInfo( tileData.circleCollider );
        } );
        drawSectionToggle( "Animation", tileData.hasAnimation, [&]() {
            DrawComponentUtils::DrawComponentInfo( tileData.animation );
        } );
        drawSectionToggle( "Physics", tileData.enablePhysics, [&]() {
            DrawComponentUtils::DrawComponentInfo( tileData.physics );
        } );

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if ( ImGui::BeginChild( "##TileLayers" ) )
        {
            ImGui::SeparatorText( "Tile Layers" );
            auto& spriteLayers = currentScene->GetLayerParams();

            if ( ImGui::Button( "Add" ) )
            {
                currentScene->AddNewLayer();
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            const float toggleReserve = ui::GetToggleSwitchWidth() + ImGui::GetStyle().ItemSpacing.x * 2.0f;
            const float itemWidth = std::max( 40.f, ImGui::GetContentRegionAvail().x - toggleReserve );
            auto rView = spriteLayers | std::ranges::views::reverse;

            for ( auto rit = rView.begin(); rit != rView.end(); rit++ )
            {
                int n = static_cast<int>( std::distance( rit, rView.end() ) ) - 1;
                ImGui::PushID( n );

                auto& spriteLayer = *rit;
                bool bIsSelected{ m_SelectedLayer == n };

                const char* layerLabel = spriteLayer.layerName.empty() ? "Layer" : spriteLayer.layerName.c_str();
                ImGui::Selectable( layerLabel, bIsSelected, 0, ImVec2{ itemWidth, 0.f } );

                if ( ImGui::IsItemActive() && !ImGui::IsItemHovered() )
                {
                    int n_next = n + ( ImGui::GetMouseDragDelta( 0 ).y < 0.f ? 1 : -1 );
                    if ( n_next >= 0 && n_next < static_cast<int>( rView.size() ) )
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
                ui::ToggleSwitch( "##visible", &spriteLayer.visible );
                ImGui::PopID();
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }
}
