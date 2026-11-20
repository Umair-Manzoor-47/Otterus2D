#include "ToolManager.h"
#include "CreateTileTool.h"
#include "RectFillTool.h"
#include "ToolAccessories.h"

namespace otterus_editor {
    ToolManager::ToolManager()
    {
        auto createTileTool = std::make_unique<CreateTileTool>();
        m_mapTools.emplace( ToolType::CREATE_TILE, std::make_unique<CreateTileTool>() );
        m_mapTools.emplace( ToolType::RECT_FILL_TILE, std::make_unique<RectFillTool>() );
        // TODO: Add other tools as per need

        SetToolActive( ToolType::RECT_FILL_TILE );
    }

    void ToolManager::Update( Canvas& canvas )
    {
        auto activeTool = std::ranges::find_if( m_mapTools, []( const auto& tool ) { return tool.second->IsActivated(); } );
        if ( activeTool != m_mapTools.end() )
            activeTool->second->Update( canvas );
    }

    void ToolManager::SetToolActive( ToolType toolType )
    {
        for ( const auto& [ eType, tool ] : m_mapTools )
        {
            if ( eType == toolType )
                tool->Activate();
            else
                tool->Deactivate();
        }
    }

    TileTool* ToolManager::GetActiveTool()
    {
        auto activeTool = std::ranges::find_if( m_mapTools, []( const auto& tool ) { return tool.second->IsActivated(); } );
        if ( activeTool != m_mapTools.end() )
            return activeTool->second.get();

        return nullptr;
    }
}
