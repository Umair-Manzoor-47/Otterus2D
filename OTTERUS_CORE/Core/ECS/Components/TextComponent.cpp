#include "TextComponent.h"
#include <entt.hpp>

void otterus_core::ECS::TextComponent::CreateLuaTextBindings(sol::state& lua)
{
	lua.new_usertype<TextComponent>(
		"TextComponent",
		"type_id", &entt::type_hash<TextComponent>::value,
		sol::call_constructor,
		sol::factories(
			[](const std::string& textStr, const std::string& fontName, int padding, float wrap, otterus_rendering::Color color) {
				return TextComponent{
					.textStr = textStr,
					.fontName = fontName,
					.padding = padding,
					.wrap = wrap,
					.color = color,
				};
			}
		),
		"textStr", &TextComponent::textStr,
		"fontName", &TextComponent::fontName,
		"padding", &TextComponent::padding,
		"wrap", &TextComponent::wrap,
		"color", &TextComponent::color,
		"isHidden", &TextComponent::isHidden
	);

}
