#pragma once

#include <sol/sol.hpp>
#include <Rendering/Essentials/Vertex.h>

namespace otterus_core::ECS {
	struct TextComponent
	{
		std::string textStr{ "" }, fontName{ "pixel" };
		int padding{ 0 };
		float wrap { -1.f };
		otterus_rendering::Color color{ 255, 255, 255, 255 };
		bool isHidden{ false };

		static void CreateLuaTextBindings(sol::state& lua);

		[[nodiscard]] std::string to_string() const;
	};

}