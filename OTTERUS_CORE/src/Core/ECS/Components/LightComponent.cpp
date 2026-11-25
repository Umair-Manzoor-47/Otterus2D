#include "Core/ECS/Components/LightComponent.h"
#include <entt.hpp>
#include <sstream>

namespace otterus_core::ECS
{
    void LightComponent::CreateLuaLightBind(sol::state& lua)
    {
        lua.new_usertype<LightComponent>(
            "Light",
            "type_id", &entt::type_hash<LightComponent>::value,
            sol::call_constructor,
            sol::factories(
                []() {
                    return LightComponent{};
                },
                [](glm::vec4 color, float intensity, float radius) {
                    return LightComponent{
                        .color = color,
                        .intensity = intensity,
                        .radius = radius
                    };
                },
                [](glm::vec4 color, float intensity, float radius, float falloff) {
                    return LightComponent{
                        .color = color,
                        .intensity = intensity,
                        .radius = radius,
                        .falloff = falloff
                    };
                },
                [](float r, float g, float b, float a, float intensity, float radius) {
                    return LightComponent{
                        .color = glm::vec4{ r, g, b, a },
                        .intensity = intensity,
                        .radius = radius
                    };
                }
            ),
            "color", &LightComponent::color,
            "intensity", &LightComponent::intensity,
            "radius", &LightComponent::radius,
            "falloff", &LightComponent::falloff,
            "isSpotlight", &LightComponent::isSpotlight,
            "innerAngle", &LightComponent::innerAngle,
            "outerAngle", &LightComponent::outerAngle,
            "castShadows", &LightComponent::castShadows,
            "to_string", &LightComponent::to_string
        );
    }

    std::string LightComponent::to_string() const
    {
        std::stringstream ss;
        ss <<
            "==== Light Component ==== \n" << std::boolalpha <<
            "Color: [ r = " << color.r << ", g = " << color.g << ", b = " << color.b << ", a = " << color.a << " ]\n" <<
            "Intensity: " << intensity << "\n" <<
            "Radius: " << radius << "\n" <<
            "Falloff: " << falloff << "\n" <<
            "IsSpotlight: " << isSpotlight << "\n" <<
            "InnerAngle: " << innerAngle << "\n" <<
            "OuterAngle: " << outerAngle << "\n" <<
            "CastShadows: " << castShadows << "\n";

        return ss.str();
    }
}
