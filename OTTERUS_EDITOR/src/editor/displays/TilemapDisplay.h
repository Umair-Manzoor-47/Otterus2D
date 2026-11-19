#pragma once
#include "IDisplay.h"
#include <string>

#include "glm/vec2.hpp"

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
        void LoadNewScene();
        void PanZoomCamera(const glm::vec2& mousePos);

    public:
        TilemapDisplay();
        ~TilemapDisplay() = default;

        virtual void Draw() override;
        virtual void Update() override;
    };
}