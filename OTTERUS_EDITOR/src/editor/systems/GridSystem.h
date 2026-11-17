#pragma once

#include <memory>

namespace otterus_rendering {
    class RectBatchRenderer;
    class Camera2D;
}

namespace otterus_editor {
    class GridSystem {
    private:
        std::unique_ptr<otterus_rendering::RectBatchRenderer> m_RectBatchRenderer;

    public:
        GridSystem();
        ~GridSystem() = default;

        void Update(otterus_rendering::Camera2D& camera);
    };
}