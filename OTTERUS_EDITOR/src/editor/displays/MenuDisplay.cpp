#include "MenuDisplay.h"
#include "Logger/Logger.h"
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
                    auto file = fd.OpenFileDialog( "Open tests", SDL_GetBasePath(), { "*.png", "*.jpg" } );

                    if (!file.empty())
                    {
                        OTTERUS_LOG( "FILE OPENED: {}", file );
                    }
                }

                otterus_filesystem::FileDialog fd{};
                auto file = fd.SaveFileDialog( "Save Tilemap test", SDL_GetBasePath(), { "*.json" } );
                if (!file.empty())
                {
                    // TODO: Save the tilemap!!!
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
