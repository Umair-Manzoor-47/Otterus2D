#pragma once
#include <map>
#include <memory>

namespace otterus_editor
{

    struct Canvas;
    class TileTool;
    enum class ToolType;

    class ToolManager
    {
    private:
        std::map<ToolType, std::unique_ptr<TileTool>> m_mapTools;

    public:
        ToolManager();
        ~ToolManager() = default;

        void Update( Canvas& canvas );

        void SetToolActive( ToolType toolType );

        TileTool* GetActiveTool();
    };
}