#include "GridSystem.h"
#include <Core/ECS/MainRegistry.h>
#include <Rendering/Core/RectBatchRenderer.h>
#include <Rendering/Core/Camera2D.h>
#include <Core/Resources/AssetManager.h>
#include <Rendering/Essentials/Vertex.h>
#include <Rendering/Essentials/Primitives.h>

namespace otterus_editor {
    GridSystem::GridSystem()
        : m_RectBatchRenderer{ std::make_unique<otterus_rendering::RectBatchRenderer>()}
    {}

    void GridSystem::Update(otterus_rendering::Camera2D &camera) {
        auto& assetManager = MAIN_REGISTRY().GetAssetManager();
        auto camMat = camera.GetCameraMatrix();

        auto& colorShader = assetManager.GetShader("color");
        colorShader.Enable();
        colorShader.SetUniformMat4("projection", camMat);
        m_RectBatchRenderer->Begin();

        constexpr int tileSize {32};
        int tileWidth{tileSize}, tileHeight{tileSize}, canvasWidth{ 640 }, canvasHeight { 480 };

        int cols = canvasWidth / tileWidth;
        int rows = canvasHeight / tileHeight;

        otterus_rendering::Color color{};

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                // Create the checkerboard colors
                if ( ( col + row ) % 2 == 0 )
                    color = { 125, 125, 125, 70 };
                else
                    color = { 200, 200, 200, 70 };

                otterus_rendering::Rect rect{
                    .position = glm::vec2{
                        static_cast<float>(col * tileWidth),
                        static_cast<float>(row * tileHeight)
                    },
                    .width = static_cast<float>(tileWidth),
                    .height = static_cast<float>(tileHeight),
                    .color = color
                };

                m_RectBatchRenderer->AddRect( rect );
            }
        }
        m_RectBatchRenderer->End();
        m_RectBatchRenderer->Render();

        colorShader.Disable();
    }
}
