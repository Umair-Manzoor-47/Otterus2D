#include "MenuDisplay.h"
#include "Logger/Logger.h"
#include "Core/Loaders/TilemapLoader.h"
#include "editor/scene/SceneManager.h"
#include "editor/scene/SceneObject.h"
#include <imgui.h>
#include <SDL.h>

#include "OtterusFileSystem/Dialogs/FileDialog.h"

namespace otterus_editor
{
    void MenuDisplay::Draw()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl + N"))
                {
                    OTTERUS_LOG( "NEW PRESSED" );
                }

                if ( ImGui::MenuItem( "Open", "Ctrl + O" ) )
                {
                    otterus_filesystem::FileDialog fd{};
                    auto file = fd.OpenFileDialog( "Open tilemap test", SDL_GetBasePath(), { "*.json" } );

                    if (!file.empty())
                    {
                        auto currentScene = SCENE_MANAGER().GetCurrentScene();
                        if ( currentScene )
                        {
                            otterus_core::Loaders::TilemapLoader tl{};
                            if ( !tl.LoadTilemap( currentScene->GetRegistry(), file, true ) )
                            {
                                OTTERUS_ERROR( "Failed to load tilemap." );
                            }
                        }
                        else
                        {
                            OTTERUS_ERROR( "Failed to load tilemap. No active scene." );
                        }
                    }
                }

                if ( ImGui::MenuItem( "Save", "Ctrl + S" ) )
                {
                    otterus_filesystem::FileDialog fd{};
                    auto file = fd.SaveFileDialog( "Save Tilemap test", SDL_GetBasePath(), { "*.json" } );
                    if (!file.empty())
                    {
                        auto currentScene = SCENE_MANAGER().GetCurrentScene();
                        if (currentScene)
                        {
                            otterus_core::Loaders::TilemapLoader tl{};
                            if (!tl.SaveTilemap(currentScene->GetRegistry(), file, true))
                            {
                                OTTERUS_ERROR( "Failed to save tilemap." );
                            }
                        }
                        else
                        {
                            OTTERUS_ERROR( "Failed to save tilemap. No active scene." );
                        }
                    }
                }

                if ( ImGui::MenuItem( "Exit") )
                {
                    OTTERUS_LOG( "SHOULD EVENTUALLY EXIT!" );
                }

                ImGui::EndMenu();
            }

            if ( ImGui::BeginMenu( "Edit" ) )
            {
                ImGui::EndMenu();
            }

            if ( ImGui::BeginMenu( "Tools" ) )
            {
                ImGui::EndMenu();
            }

            if ( ImGui::BeginMenu( "Settings" ) )
            {
                ImGui::EndMenu();
            }

            if ( ImGui::BeginMenu( "Help" ) )
            {
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }
}
