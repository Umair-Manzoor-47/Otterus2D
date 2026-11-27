#include "EditorTheme.h"

#include <SDL.h>

#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

namespace otterus_editor
{
    namespace
    {
        constexpr ImVec4 Hex( unsigned int rgb, float alpha = 1.0f )
        {
            return ImVec4{
                static_cast<float>( ( rgb >> 16 ) & 0xFF ) / 255.0f,
                static_cast<float>( ( rgb >> 8 ) & 0xFF ) / 255.0f,
                static_cast<float>( rgb & 0xFF ) / 255.0f,
                alpha
            };
        }

        struct ThemePalette
        {
            ImVec4 gutterBg        = Hex( 0x15181c );
            ImVec4 panelBg         = Hex( 0x22272e );
            ImVec4 childBg         = Hex( 0x181c21 );
            ImVec4 surfaceElevated = Hex( 0x283038 );
            ImVec4 frameBg         = Hex( 0x1d2228 );
            ImVec4 frameHover      = Hex( 0x262e36 );
            ImVec4 frameActive     = Hex( 0x2d3640 );
            ImVec4 border          = Hex( 0x323c46 );
            ImVec4 borderSubtle    = Hex( 0x262e36 );
            ImVec4 textMain        = Hex( 0xe2ded7 );
            ImVec4 textMuted       = Hex( 0x828e9b );
            ImVec4 teal            = Hex( 0x48a999 );
            ImVec4 tealHover       = Hex( 0x63c4b4 );
            ImVec4 amber           = Hex( 0xd68d4e );
            ImVec4 amberHover      = Hex( 0xe5a86d );
            ImVec4 toggleThumb     = Hex( 0xf5f0e8 );
            ImVec4 infoColor       = Hex( 0xf0f2f5 );
            ImVec4 warnColor       = Hex( 0xf5c842 );
            ImVec4 errorColor      = Hex( 0xeb5757 );
        };

        ThemeTokens BuildTokens( const ThemePalette& p )
        {
            ThemeTokens tokens{};
            tokens.accentTeal      = p.teal;
            tokens.accentTealHover = p.tealHover;
            tokens.toggleThumb     = p.toggleThumb;
            tokens.selectionBg     = ImVec4{ p.teal.x, p.teal.y, p.teal.z, 0.30f };
            tokens.playActiveBg    = ImVec4{ p.teal.x, p.teal.y, p.teal.z, 0.45f };
            tokens.stopActiveBg    = ImVec4{ p.amber.x, p.amber.y, p.amber.z, 0.38f };

            tokens.filterInfoActiveBg  = Hex( 0x4d5763 );
            tokens.filterWarnActiveBg  = ImVec4{ p.warnColor.x * 0.55f, p.warnColor.y * 0.45f, p.warnColor.z * 0.25f, 1.0f };
            tokens.filterErrorActiveBg = ImVec4{ p.errorColor.x * 0.55f, p.errorColor.y * 0.30f, p.errorColor.z * 0.30f, 1.0f };

            tokens.infoColor     = p.infoColor;
            tokens.warnColor     = p.warnColor;
            tokens.errorColor    = p.errorColor;
            tokens.mutedText     = p.textMuted;
            tokens.highlightText = p.tealHover;
            return tokens;
        }

        ThemeTokens g_Tokens = BuildTokens( ThemePalette{} );
    }

    void EditorTheme::Apply()
    {
        // Riverbed Slate palette (matched to Otterus2D-docs custom.css)
        const ThemePalette p{};

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding            = ImVec2{ 8.0f, 8.0f };
        style.FramePadding             = ImVec2{ 6.0f, 4.0f };
        style.CellPadding              = ImVec2{ 4.0f, 3.0f };
        style.ItemSpacing              = ImVec2{ 6.0f, 5.0f };
        style.ItemInnerSpacing         = ImVec2{ 5.0f, 4.0f };
        style.IndentSpacing            = 14.0f;
        style.ScrollbarSize            = 10.0f;
        style.GrabMinSize              = 8.0f;
        style.WindowBorderSize         = 1.0f;
        style.ChildBorderSize          = 1.0f;
        style.PopupBorderSize          = 1.0f;
        style.FrameBorderSize          = 1.0f;
        style.TabBorderSize            = 0.0f;
        style.TabBarBorderSize         = 1.0f;
        style.TabBarOverlineSize       = 2.0f;
        style.DockingSeparatorSize     = 2.0f;
        style.WindowRounding           = 2.0f;
        style.ChildRounding            = 3.0f;
        style.FrameRounding            = 3.0f;
        style.PopupRounding            = 3.0f;
        style.ScrollbarRounding        = 5.0f;
        style.GrabRounding             = 3.0f;
        style.TabRounding              = 3.0f;
        style.WindowTitleAlign         = ImVec2{ 0.02f, 0.5f };
        style.WindowMenuButtonPosition = ImGuiDir_None;
        style.ColorButtonPosition      = ImGuiDir_Right;
        style.SeparatorTextBorderSize  = 1.0f;
        style.SeparatorTextPadding     = ImVec2{ 12.0f, 3.0f };

        ImVec4* c = style.Colors;

        // Text
        c[ImGuiCol_Text]             = p.textMain;
        c[ImGuiCol_TextDisabled]     = p.textMuted;
        c[ImGuiCol_TextLink]         = p.tealHover;
        c[ImGuiCol_TextSelectedBg]   = ImVec4{ p.teal.x, p.teal.y, p.teal.z, 0.30f };
        c[ImGuiCol_InputTextCursor]  = p.amberHover;

        // Backgrounds
        c[ImGuiCol_WindowBg]         = p.panelBg;
        c[ImGuiCol_ChildBg]          = p.childBg;
        c[ImGuiCol_PopupBg]          = ImVec4{ p.gutterBg.x, p.gutterBg.y, p.gutterBg.z, 0.98f };
        c[ImGuiCol_MenuBarBg]        = p.gutterBg;
        c[ImGuiCol_DockingEmptyBg]   = p.gutterBg;

        // Borders & Separators
        c[ImGuiCol_Border]           = p.border;
        c[ImGuiCol_BorderShadow]     = ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };
        c[ImGuiCol_Separator]        = p.border;
        c[ImGuiCol_SeparatorHovered] = p.teal;
        c[ImGuiCol_SeparatorActive]  = p.tealHover;

        // Title Bars
        c[ImGuiCol_TitleBg]          = p.gutterBg;
        c[ImGuiCol_TitleBgActive]    = p.childBg;
        c[ImGuiCol_TitleBgCollapsed] = p.gutterBg;

        // Frames & Controls
        c[ImGuiCol_FrameBg]              = p.frameBg;
        c[ImGuiCol_FrameBgHovered]       = p.frameHover;
        c[ImGuiCol_FrameBgActive]        = p.frameActive;
        c[ImGuiCol_ScrollbarBg]          = p.gutterBg;
        c[ImGuiCol_ScrollbarGrab]        = p.border;
        c[ImGuiCol_ScrollbarGrabHovered] = p.textMuted;
        c[ImGuiCol_ScrollbarGrabActive]  = p.teal;
        c[ImGuiCol_CheckMark]            = p.tealHover;
        c[ImGuiCol_CheckboxSelectedBg]   = ImVec4{ p.teal.x, p.teal.y, p.teal.z, 0.25f };
        c[ImGuiCol_SliderGrab]           = p.amber;
        c[ImGuiCol_SliderGrabActive]     = p.amberHover;

        // Buttons & Headers
        c[ImGuiCol_Button]        = p.surfaceElevated;
        c[ImGuiCol_ButtonHovered] = ImVec4{ p.teal.x, p.teal.y, p.teal.z, 0.28f };
        c[ImGuiCol_ButtonActive]  = ImVec4{ p.teal.x, p.teal.y, p.teal.z, 0.45f };
        c[ImGuiCol_Header]        = ImVec4{ p.teal.x, p.teal.y, p.teal.z, 0.22f };
        c[ImGuiCol_HeaderHovered] = ImVec4{ p.teal.x, p.teal.y, p.teal.z, 0.34f };
        c[ImGuiCol_HeaderActive]  = ImVec4{ p.teal.x, p.teal.y, p.teal.z, 0.48f };

        // Tabs & Docking
        c[ImGuiCol_Tab]                       = p.gutterBg;
        c[ImGuiCol_TabHovered]                = p.frameHover;
        c[ImGuiCol_TabSelected]               = p.panelBg;
        c[ImGuiCol_TabSelectedOverline]       = p.tealHover;
        c[ImGuiCol_TabDimmed]                 = p.gutterBg;
        c[ImGuiCol_TabDimmedSelected]         = p.childBg;
        c[ImGuiCol_TabDimmedSelectedOverline] = ImVec4{ p.teal.x, p.teal.y, p.teal.z, 0.50f };
        c[ImGuiCol_DockingPreview]            = ImVec4{ p.teal.x, p.teal.y, p.teal.z, 0.55f };
        c[ImGuiCol_DragDropTarget]            = p.amberHover;
        c[ImGuiCol_DragDropTargetBg]          = ImVec4{ p.amber.x, p.amber.y, p.amber.z, 0.20f };

        // Tables & Misc
        c[ImGuiCol_TableHeaderBg]         = p.surfaceElevated;
        c[ImGuiCol_TableBorderStrong]     = p.border;
        c[ImGuiCol_TableBorderLight]      = p.borderSubtle;
        c[ImGuiCol_TableRowBg]            = ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };
        c[ImGuiCol_TableRowBgAlt]         = ImVec4{ p.gutterBg.x, p.gutterBg.y, p.gutterBg.z, 0.45f };
        c[ImGuiCol_ResizeGrip]            = ImVec4{ p.border.x, p.border.y, p.border.z, 0.40f };
        c[ImGuiCol_ResizeGripHovered]     = p.teal;
        c[ImGuiCol_ResizeGripActive]      = p.tealHover;
        c[ImGuiCol_PlotLines]             = p.tealHover;
        c[ImGuiCol_PlotLinesHovered]      = p.amberHover;
        c[ImGuiCol_PlotHistogram]         = p.amber;
        c[ImGuiCol_PlotHistogramHovered]  = p.amberHover;
        c[ImGuiCol_TreeLines]             = p.border;
        c[ImGuiCol_UnsavedMarker]         = p.amberHover;
        c[ImGuiCol_NavCursor]             = p.tealHover;
        c[ImGuiCol_NavWindowingHighlight] = p.tealHover;
        c[ImGuiCol_NavWindowingDimBg]     = ImVec4{ 0.05f, 0.05f, 0.05f, 0.55f };
        c[ImGuiCol_ModalWindowDimBg]      = ImVec4{ 0.04f, 0.04f, 0.04f, 0.65f };

        if ( ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
        {
            c[ImGuiCol_WindowBg].w = 1.0f;
        }

        g_Tokens = BuildTokens( p );
    }

    void EditorTheme::ConfigureFonts( ImGuiIO& io )
    {
        namespace fs = std::filesystem;

        std::vector<fs::path> candidates;
        if ( char* basePath = SDL_GetBasePath() )
        {
            const fs::path base{ basePath };
            SDL_free( basePath );
            candidates.push_back( base / "assets/fonts/Inter-Regular.ttf" );
            candidates.push_back( base / "assets/fonts/Roboto-Regular.ttf" );
        }

        candidates.emplace_back( "assets/fonts/Inter-Regular.ttf" );
        candidates.emplace_back( "assets/fonts/Roboto-Regular.ttf" );

#ifdef _WIN32
        if ( const char* winDir = std::getenv( "WINDIR" ) )
        {
            candidates.push_back( fs::path{ winDir } / "Fonts/segoeui.ttf" );
        }
        else
        {
            candidates.emplace_back( "C:/Windows/Fonts/segoeui.ttf" );
        }
#elif defined( __APPLE__ )
        candidates.emplace_back( "/System/Library/Fonts/SFNS.ttf" );
        candidates.emplace_back( "/Library/Fonts/Arial.ttf" );
#else
        candidates.emplace_back( "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf" );
        candidates.emplace_back( "/usr/share/fonts/TTF/DejaVuSans.ttf" );
#endif

        for ( const auto& path : candidates )
        {
            std::error_code ec;
            if ( fs::exists( path, ec ) )
            {
                ImFontConfig cfg{};
                cfg.OversampleH = 2;
                cfg.OversampleV = 2;
                cfg.PixelSnapH = true;
                const std::string utf8Path = path.string();
                if ( io.Fonts->AddFontFromFileTTF( utf8Path.c_str(), 19.0f, &cfg ) != nullptr )
                {
                    return;
                }
            }
        }

        io.Fonts->AddFontDefault();
    }

    const ThemeTokens& EditorTheme::GetTokens()
    {
        return g_Tokens;
    }
}
