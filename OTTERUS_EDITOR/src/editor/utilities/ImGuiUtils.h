#pragma once

#include <imgui.h>
#include <cstdint>

namespace otterus_editor::ui
{
    constexpr ImGuiColorEditFlags COLOR_PICKER_FLAGS =
        ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB;

    float GetToggleSwitchWidth();
    bool ToggleSwitch( const char* label, bool* v );
    bool ComponentSectionHeader( const char* label, ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen );
    bool BeginPropertyTable( const char* tableId );
    void PropertyLabel( const char* label );
    void EndPropertyTable();

    bool DragFloat2Axis( const char* id, float* x, float* y, float speed = 0.5f,
                         float minVal = 0.0f, float maxVal = 0.0f, const char* format = "%.1f" );
    bool DragInt2Axis( const char* id, int* x, int* y, float speed = 0.25f,
                       int minVal = 0, int maxVal = 0 );
    bool ColorEdit4U8( const char* id, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a );
}
