#pragma once
#include "IDisplay.h"

namespace otterus_core::ECS {
    class Entity;
}

namespace otterus_editor{
    class SceneHierarchyDisplay : public IDisplay
    {
    private:
        std::shared_ptr<otterus_core::ECS::Entity> m_SelectedEntity {nullptr};
        bool m_AddComponent{false};
    private:
        bool OpenTreeNode(otterus_core::ECS::Entity& entity);
        bool AddComponent(otterus_core::ECS::Entity& entity, bool* addComponent );

        void DrawGameObjectDetails();
        void DrawEntityComponents();

    public:
        SceneHierarchyDisplay();
        ~SceneHierarchyDisplay();

        virtual void Update() override;;
        virtual void Draw() override;
    };

}
