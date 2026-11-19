#include "AbstractTool.h"
#include "Logger/Logger.h"
#include "Core/Scripting/InputManager.h"
#include "Core/ECS/Registry.h"
#include "Rendering/Core/Camera2D.h"
#include "editor/utilities/EditorUtilities.h"

namespace otterus_editor
{

    void AbstractTool::UpdateMouseWorldCoords()
    {
        m_MouseScreenCoords = m_GUICursorCoords - m_GUIRelativeCoords;
        if ( !m_Camera )
            return;

        m_MouseWorldCoords = m_Camera->ScreenCoordToWorld( m_MouseScreenCoords );
    }

    void AbstractTool::CheckOutOfBounds( const Canvas& canvas )
    {
        auto boundsWidth{ canvas.width - ( canvas.tileWidth * 0.5f ) };
        auto boundsHeight{ canvas.height - ( canvas.tileHeight * 0.5f ) };

        if ( m_WindowPos.x <= m_GUICursorCoords.x && m_WindowPos.x + m_WindowSize.x >= m_GUICursorCoords.x &&
             m_WindowPos.y <= m_GUICursorCoords.y && m_WindowPos.y + m_WindowSize.y >= m_GUICursorCoords.y &&
             m_MouseScreenCoords.x > 0.f && m_MouseScreenCoords.y > 0.f && m_MouseScreenCoords.x < m_WindowSize.x &&
             m_MouseScreenCoords.y < m_WindowSize.y && m_MouseWorldCoords.x <= boundsWidth &&
             m_MouseWorldCoords.y <= boundsHeight && m_MouseWorldCoords.x >= 0.f && m_MouseWorldCoords.y >= 0.f )
        {
            m_OutOfBounds = false;
        }
        else
        {
            m_OutOfBounds = true;
        }
    }

    bool AbstractTool::MouseBtnJustPressed( EMouseButton button )
    {
        return INPUT_MANAGER().GetMouse().IsBtnJustPressed( static_cast<int>( button ) );
    }

    bool AbstractTool::MouseBtnJustReleased( EMouseButton button )
    {
        return INPUT_MANAGER().GetMouse().IsBtnJustReleased( static_cast<int>( button ) );
    }

    bool AbstractTool::MouseBtnPressed( EMouseButton button )
    {
        return INPUT_MANAGER().GetMouse().IsBtnPressed( static_cast<int>( button ) );
    }

    bool AbstractTool::MouseMoving()
    {
        return INPUT_MANAGER().GetMouse().GetMouseMoving();
    }

    AbstractTool::AbstractTool()
        : m_MouseScreenCoords{ 0.f }
    , m_MouseWorldCoords{ 0.f }
    , m_GUICursorCoords{ 0.f }
    , m_GUIRelativeCoords{ 0.f }
    , m_WindowPos{ 0.f }
    , m_WindowSize{ 0.f }
    , m_Activated{ false }
    , m_OutOfBounds{ false }
    {
    }

    void AbstractTool::Update( Canvas& canvas )
    {
        UpdateMouseWorldCoords();
        CheckOutOfBounds( canvas );
    }

    bool AbstractTool::SetupTool( otterus_core::ECS::Registry* registry, otterus_rendering::Camera2D* camera )
    {
        if ( !registry )
        {
            OTTERUS_ERROR( "Failed to setup tool - Registry was nullptr." );
            return false;
        }
        if ( !camera )
        {
            OTTERUS_ERROR( "Failed to setup tool - Camera was nullptr." );
            return false;
        }

        m_Camera = camera;
        m_Registry = registry;

        return true;
    }

}