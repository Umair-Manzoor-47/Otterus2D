#include "TextComponent.h"
#include <entt.hpp>

namespace otterus_core::ECS {
	void TextComponent::CreateLuaTextBindings(sol::state& lua)
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
			"isHidden", &TextComponent::isHidden,
			"to_string", &TextComponent::to_string
		);

	}

	std::string TextComponent::to_string() const
	{
		std::stringstream ss;
		ss <<
			"==== Text Component ==== \n" << std::boolalpha <<
			"Text: " << textStr << "\n" <<
			"Font Name: " << fontName << "\n" <<
			"Padding: " << padding << "\n" <<
			"Wrap: " << wrap << "\n" <<
			"isHidden: " << isHidden << "\n" <<
			"Color: \n\t" <<
			"Red: " << color.r << "\n\t" <<
			"Green: " << color.g << "\n\t" <<
			"Blue: " << color.b << "\n\t" <<
			"Alpha: " << color.a << "\n";

		return ss.str();
	}
}