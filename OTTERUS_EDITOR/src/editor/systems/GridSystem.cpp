#include "GridSystem.h"
#include <Core/ECS/MainRegistry.h>
#include <Rendering/Core/RectBatchRenderer.h>
#include <Rendering/Core/Camera2D.h>
#include <Core/Resources/AssetManager.h>
#include <Rendering/Essentials/Vertex.h>
#include <Rendering/Essentials/Primitives.h>
#include "editor/scene/SceneObject.h"

namespace otterus_editor {
    GridSystem::GridSystem()
        : m_RectBatchRenderer{ std::make_unique<otterus_rendering::RectBatchRenderer>()}
    {}

    void GridSystem::Update(SceneObject& currentScene, otterus_rendering::Camera2D &camera) {
        auto& assetManager = MAIN_REGISTRY().GetAssetManager();
        const auto& canvas = currentScene.GetCanvas();
        auto camMat = camera.GetCameraMatrix();

        auto colorShader = assetManager.GetShader("color");
        if (!colorShader)
            return;
        colorShader->Enable();
        colorShader->SetUniformMat4("uProjection", camMat);
        m_RectBatchRenderer->Begin();

        int tileWidth{ canvas.tileWidth }, tileHeight{ canvas.tileHeight };
        int canvasWidth{ canvas.width }, canvasHeight { canvas.height };

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

        colorShader->Disable();
    }
}
