#pragma once
#include <sol/sol.hpp>

namespace otterus_core { namespace ECS { class Registry; } }

namespace otterus_core::Scripting {

	struct RendererBinder
	{
		static void CreateRendererBind(sol::state& lua, otterus_core::ECS::Registry& registry);

	};

}