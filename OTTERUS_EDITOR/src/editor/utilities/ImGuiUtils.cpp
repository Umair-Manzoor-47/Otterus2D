#include "ImGuiUtils.h"

#include "ImGuiUtils.h"
#include "Logger/Logger.h"

namespace ImGui
{
    void ColoredLabel( const std::string& label, const ImVec2& size, const ImVec4& color )
    {
        PushStyleColor( ImGuiCol_Button, color );
        PushStyleColor( ImGuiCol_ButtonHovered, color );
        PushStyleColor( ImGuiCol_ButtonActive, color );
        Button( label.c_str(), size );
        PopStyleColor();
        PopStyleColor();
        PopStyleColor();
    }

    void OffsetTextX( const std::string& label, float position )
    {
        SetCursorPosX( position );
        Text( label.c_str() );
    }

    void AddSpaces( int numSpaces )
    {
        OTTERUS_ASSERT( numSpaces > 0 && "Number of spaces must be a positive number!" );
        for ( int i = 0; i < numSpaces; ++i )
            Spacing();
    }

    void InlineLabel( const std::string& label, float spaceSize )
    {
        Text( label.c_str() );
        SameLine();
        SetCursorPosX( spaceSize );
    }
}