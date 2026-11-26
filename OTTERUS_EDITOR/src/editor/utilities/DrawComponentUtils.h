#pragma once
#include "Core/ECS/Components/AllComponents.h"

namespace otterus_core::ECS {
    class Entity;
}

namespace otterus_editor{
    class DrawComponentUtils {

    public:
        DrawComponentUtils() = delete;

        template <typename TComponent>
        static void DrawEntityComponentInfo(otterus_core::ECS::Entity& entity);

        template <typename TComponent>
        static void DrawComponentInfo(TComponent& component);

        template <typename TComponent>
        static void RegisterUIComponent();

    private:
        static void DrawImGuiComponent( otterus_core::ECS::TransformComponent& transform );
        static void DrawImGuiComponent( otterus_core::ECS::SpriteComponent& sprite );
        static void DrawImGuiComponent( otterus_core::ECS::AnimationComponent& animation);
        static void DrawImGuiComponent( otterus_core::ECS::BoxColliderComponent& boxCollider);
        static void DrawImGuiComponent( otterus_core::ECS::CircleColliderComponent& circleCollider);
        static void DrawImGuiComponent( otterus_core::ECS::PhysicsComponent& physics);
        static void DrawImGuiComponent( otterus_core::ECS::RigidBodyComponent& rigidbody);
        static void DrawImGuiComponent( otterus_core::ECS::TextComponent& textComponent);
        static void DrawImGuiComponent( otterus_core::ECS::Identification& identification);
        static void DrawImGuiComponent( otterus_core::ECS::TileComponent& tileComponent);
        static void DrawImGuiComponent( otterus_core::ECS::LightComponent& light);

    };
}

#include "DrawComponentUtils.inl"