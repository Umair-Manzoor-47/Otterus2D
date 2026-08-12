#pragma once
#include <sol/sol.hpp>

namespace otterus_core { namespace ECS { class Registry; } }

namespace otterus_core::Scripting {

	struct SoundBindings
	{
		static void CreateSoundBind(sol::state& lua, otterus_core::ECS::Registry& registry);

	};

}