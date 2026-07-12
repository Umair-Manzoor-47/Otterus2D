#pragma once

#include "../ECS/Registry.h"
#include <sol/sol.hpp>

namespace otterus_core::Systems {


	class ScriptingSystem
	{

	private:
		otterus_core::ECS::Registry& m_registry;
		bool m_mainLoaded;

	public:
		ScriptingSystem(otterus_core::ECS::Registry& registry);
		~ScriptingSystem() = default;

		bool LoadMainScript(sol::state& lua);
		void Update();
		void Render();
		

		static void RegisterLuaBindings(sol::state& lua, otterus_core::ECS::Registry& registry);
	};

	
}
