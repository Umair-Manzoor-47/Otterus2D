#pragma once
#include <sol/sol.hpp>

namespace otterus_core::Scripting {

	struct GLMBindings
	{
		static void CreateGLMBindings(sol::state& lua);
	};

}