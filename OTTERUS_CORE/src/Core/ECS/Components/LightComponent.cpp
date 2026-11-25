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
                [](const otterus_rendering::Color& color, float intensity, float radius) {
                    return LightComponent{
                        .color = color,
                        .intensity = intensity,
                        .radius = radius
                    };
                },
                [](const otterus_rendering::Color& color, float intensity, float radius, float falloff) {
                    return LightComponent{
                        .color = color,
                        .intensity = intensity,
                        .radius = radius,
                        .falloff = falloff
                    };
                },
                [](const otterus_rendering::Color& color, float intensity, float radius, float falloff, const glm::vec2& offset) {
                    return LightComponent{
                        .color = color,
                        .offset = offset,
                        .intensity = intensity,
                        .radius = radius,
                        .falloff = falloff
                    };
                },
                [](GLubyte r, GLubyte g, GLubyte b, GLubyte a, float intensity, float radius) {
                    return LightComponent{
                        .color = otterus_rendering::Color{ r, g, b, a },
                        .intensity = intensity,
                        .radius = radius
                    };
                }
            ),
            "color", &LightComponent::color,
            "offset", &LightComponent::offset,
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
            "Color: [ r = " << static_cast<int>(color.r)
            << ", g = " << static_cast<int>(color.g)
            << ", b = " << static_cast<int>(color.b)
            << ", a = " << static_cast<int>(color.a) << " ]\n" <<
            "Offset: [ x = " << offset.x << ", y = " << offset.y << " ]\n" <<
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
