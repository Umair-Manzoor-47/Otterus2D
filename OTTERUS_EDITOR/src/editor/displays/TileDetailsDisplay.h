#pragma once

#include "IDisplay.h"
#include <string>

namespace otterus_core::ECS
{
    class SpriteComponent;
}

namespace otterus_editor
{
    class SceneObject;

    class TileDetailsDisplay: public IDisplay
    {
    private:
        int m_SelectedLayer;
        std::string m_RenameLayerBuf;
        bool m_Rename;

    private:
        void DrawSpriteComponent(otterus_core::ECS::SpriteComponent& sprite, SceneObject* scene);

    public:
        TileDetailsDisplay();
        ~TileDetailsDisplay() override;

        virtual void Draw() override;
    };
}