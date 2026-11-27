#pragma once

#include <imgui.h>

namespace otterus_editor
{
    struct ThemeTokens
    {
        ImVec4 accentTeal{ 0.28f, 0.66f, 0.60f, 1.0f };
        ImVec4 accentTealHover{ 0.39f, 0.77f, 0.71f, 1.0f };
        ImVec4 toggleThumb{ 0.96f, 0.94f, 0.91f, 1.0f };

        ImVec4 selectionBg{ 0.28f, 0.66f, 0.60f, 0.30f };
        ImVec4 playActiveBg{ 0.28f, 0.66f, 0.60f, 0.45f };
        ImVec4 stopActiveBg{ 0.84f, 0.55f, 0.31f, 0.38f };

        ImVec4 filterInfoActiveBg{ 0.30f, 0.34f, 0.39f, 1.0f };
        ImVec4 filterWarnActiveBg{ 0.53f, 0.35f, 0.11f, 1.0f };
        ImVec4 filterErrorActiveBg{ 0.51f, 0.10f, 0.10f, 1.0f };

        ImVec4 infoColor{ 0.94f, 0.95f, 0.96f, 1.0f };
        ImVec4 warnColor{ 0.96f, 0.78f, 0.26f, 1.0f };
        ImVec4 errorColor{ 0.92f, 0.34f, 0.34f, 1.0f };
        ImVec4 mutedText{ 0.51f, 0.56f, 0.61f, 1.0f };
        ImVec4 highlightText{ 0.39f, 0.77f, 0.71f, 1.0f };
    };

    class EditorTheme
    {
    public:
        static void Apply();
        static void ConfigureFonts( ImGuiIO& io );
        static const ThemeTokens& GetTokens();
    };
}
