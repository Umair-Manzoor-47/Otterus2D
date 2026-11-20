#pragma once
#include "TileTool.h"
#include <memory>

namespace otterus_rendering {
    class RectBatchRenderer;
    struct Rect;
}

namespace otterus_editor {

    class RectFillTool : public TileTool
    {
    private:
        std::unique_ptr<otterus_rendering::RectBatchRenderer> m_ShapeRenderer;
        std::unique_ptr<otterus_rendering::Rect> m_TileFillRect;
        glm::vec2 m_StartPressPos;

    private:
        void CreateTiles();
        void RemoveTiles();
        void DrawPreview( int dx, int dy );
        void ResetTile();

    public:
        RectFillTool();
        ~RectFillTool();

        virtual void Create() override;
        virtual void Draw() override;
    };

}
