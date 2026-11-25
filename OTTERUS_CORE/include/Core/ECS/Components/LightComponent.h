#pragma once
#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include <string>

namespace otterus_core::ECS
{
    struct LightComponent
    {
        glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

        float intensity{ 1.0f };
        float radius{ 150.0f };

        // Exponent for pow(1.0 - d, falloff)
        float falloff{ 2.0f };

        bool isSpotlight{ false };

        // Uses TransformComponent::rotation for direction
        float innerAngle{ 15.0f }, outerAngle{ 30.0f };

        bool castShadows{ false };

        static void CreateLuaLightBind(sol::state& lua);
        [[nodiscard]] std::string to_string() const;

    };
}