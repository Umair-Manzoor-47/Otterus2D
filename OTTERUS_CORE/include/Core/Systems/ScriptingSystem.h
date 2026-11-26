#pragma once

#include <sol/sol.hpp>

namespace otterus_core::ECS
{
	class Registry;
}

namespace otterus_core::Systems {


	class ScriptingSystem
	{

	private:
		bool m_mainLoaded;

	public:
		ScriptingSystem();
		~ScriptingSystem() = default;

		bool LoadMainScript(sol::state& lua, ECS::Registry& registry);
		void Update(ECS::Registry& registry);
		void Render(ECS::Registry& registry);
		

		static void RegisterLuaBindings(sol::state& lua, ECS::Registry& registry);

		static void RegisterLuaFunctions(sol::state& lua, ECS::Registry& registry);
	};

	
}
