#include "ImGuiUtils.h"
#include "editor/utilities/EditorTheme.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstring>

namespace otterus_editor::ui
{
    namespace
    {
        constexpr float kToggleHeightRatio = 0.72f;
        constexpr float kToggleAspectRatio = 1.85f;

        template <typename DrawAxisFn>
        bool DrawAxisPair( const char* id, DrawAxisFn&& drawAxis )
        {
            bool changed = false;
            ImGui::PushID( id );

            const float avail = ImGui::GetContentRegionAvail().x;
            const float axisW = ImGui::CalcTextSize( "X" ).x + 4.0f;
            constexpr float gap = 8.0f;
            const float fieldW = std::max( 16.0f, ( avail - axisW * 2.0f - gap ) * 0.5f );

            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted( "X" );
            ImGui::SameLine( 0.0f, 4.0f );
            ImGui::SetNextItemWidth( fieldW );
            changed |= drawAxis( "##x", true );

            ImGui::SameLine( 0.0f, gap );

            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted( "Y" );
            ImGui::SameLine( 0.0f, 4.0f );
            ImGui::SetNextItemWidth( fieldW );
            changed |= drawAxis( "##y", false );

            ImGui::PopID();
            return changed;
        }
    }

    float GetToggleSwitchWidth()
    {
        return ImGui::GetFrameHeight() * kToggleHeightRatio * kToggleAspectRatio;
    }

    bool ToggleSwitch( const char* label, bool* v )
    {
        if ( !v )
            return false;

        const auto& tokens = EditorTheme::GetTokens();
        const float frameHeight = ImGui::GetFrameHeight();
        const float height = frameHeight * kToggleHeightRatio;
        const float width = height * kToggleAspectRatio;
        const float radius = height * 0.5f;

        const char* hashPos = std::strstr( label, "##" );
        const char* labelEnd = hashPos ? hashPos : ( label + std::strlen( label ) );
        const bool hasVisibleLabel = labelEnd > label;
        const float innerSpacing = ImGui::GetStyle().ItemInnerSpacing.x;
        const float labelWidth = hasVisibleLabel
            ? ( innerSpacing + ImGui::CalcTextSize( label, labelEnd ).x )
            : 0.0f;

        const ImVec2 p = ImGui::GetCursorScreenPos();
        const float yOffset = ( frameHeight - height ) * 0.5f;

        const bool clicked = ImGui::InvisibleButton( label, ImVec2{ width + labelWidth, frameHeight } );
        if ( clicked )
            *v = !*v;

        const bool hovered = ImGui::IsItemHovered();
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        const ImVec2 trackMin{ p.x, p.y + yOffset };
        const ImVec2 trackMax{ p.x + width, p.y + yOffset + height };

        const ImU32 trackCol = *v
            ? ImGui::GetColorU32( hovered ? tokens.accentTealHover : tokens.accentTeal )
            : ImGui::GetColorU32( hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg );

        drawList->AddRectFilled( trackMin, trackMax, trackCol, radius );
        if ( !*v )
        {
            drawList->AddRect( trackMin, trackMax, ImGui::GetColorU32( ImGuiCol_Border ), radius );
        }

        const float thumbRadius = radius - 2.0f;
        const float thumbX = *v ? ( trackMax.x - radius ) : ( trackMin.x + radius );
        const float thumbY = trackMin.y + radius;
        const ImU32 thumbCol = *v
            ? ImGui::GetColorU32( tokens.toggleThumb )
            : ImGui::GetColorU32( hovered ? ImGuiCol_Text : ImGuiCol_TextDisabled );

        drawList->AddCircleFilled( ImVec2{ thumbX, thumbY }, thumbRadius, thumbCol );

        if ( hasVisibleLabel )
        {
            const float textY = p.y + ImGui::GetStyle().FramePadding.y;
            drawList->AddText(
                ImVec2{ p.x + width + innerSpacing, textY },
                ImGui::GetColorU32( ImGuiCol_Text ),
                label,
                labelEnd
            );
        }

        return clicked;
    }

    bool ComponentSectionHeader( const char* label, ImGuiTreeNodeFlags flags )
    {
        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ 6.0f, 5.0f } );
        const bool open = ImGui::CollapsingHeader( label, flags );
        ImGui::PopStyleVar();
        return open;
    }

    bool BeginPropertyTable( const char* tableId )
    {
        constexpr ImGuiTableFlags flags =
            ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoSavedSettings;
        if ( !ImGui::BeginTable( tableId, 2, flags ) )
            return false;

        const float labelColWidth = std::max( 100.0f, ImGui::GetFontSize() * 6.5f );
        ImGui::TableSetupColumn( "##PropLabel", ImGuiTableColumnFlags_WidthFixed, labelColWidth );
        ImGui::TableSetupColumn( "##PropValue", ImGuiTableColumnFlags_WidthStretch );
        return true;
    }

    void PropertyLabel( const char* label )
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex( 0 );
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted( label );
        ImGui::TableSetColumnIndex( 1 );
        ImGui::SetNextItemWidth( -FLT_MIN );
    }

    void EndPropertyTable()
    {
        ImGui::EndTable();
    }

    bool DragFloat2Axis( const char* id, float* x, float* y, float speed,
                         float minVal, float maxVal, const char* format )
    {
        const ImGuiSliderFlags flags = ( minVal < maxVal ) ? ImGuiSliderFlags_AlwaysClamp : ImGuiSliderFlags_None;
        return DrawAxisPair( id, [&]( const char* axisId, bool isX ) {
            return ImGui::DragFloat( axisId, isX ? x : y, speed, minVal, maxVal, format, flags );
        } );
    }

    bool DragInt2Axis( const char* id, int* x, int* y, float speed, int minVal, int maxVal )
    {
        const ImGuiSliderFlags flags = ( minVal < maxVal ) ? ImGuiSliderFlags_AlwaysClamp : ImGuiSliderFlags_None;
        return DrawAxisPair( id, [&]( const char* axisId, bool isX ) {
            return ImGui::DragInt( axisId, isX ? x : y, speed, minVal, maxVal, "%d", flags );
        } );
    }

    bool ColorEdit4U8( const char* id, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a )
    {
        float col[4] = { r / 255.f, g / 255.f, b / 255.f, a / 255.f };
        if ( !ImGui::ColorEdit4( id, col, COLOR_PICKER_FLAGS ) )
            return false;

        auto toU8 = []( float c ) {
            return static_cast<uint8_t>( std::lround( std::clamp( c, 0.0f, 1.0f ) * 255.0f ) );
        };

        r = toU8( col[0] );
        g = toU8( col[1] );
        b = toU8( col[2] );
        a = toU8( col[3] );
        return true;
    }
}
