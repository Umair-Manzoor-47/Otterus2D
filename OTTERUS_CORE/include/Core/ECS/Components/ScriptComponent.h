#pragma once
#include <sol/sol.hpp>

namespace otterus_core::ECS {

	struct ScriptComponent
	{
		sol::protected_function update{ sol::lua_nil }, render{ sol::lua_nil };
	};
}