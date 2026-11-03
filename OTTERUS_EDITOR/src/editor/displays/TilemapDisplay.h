#pragma once
#include "IDisplay.h"
#include <string>

namespace otterus_rendering
{
    class Camera2D;
}

namespace otterus_editor
{
    class TilemapDisplay : public IDisplay
    {
    private:
        std::unique_ptr<otterus_rendering::Camera2D> m_pTilemapCam;

    private:
        void RenderTilemap();

    public:
        TilemapDisplay();
        ~TilemapDisplay() = default;

        virtual void Draw() override;
        virtual void Update() override;
    };
}