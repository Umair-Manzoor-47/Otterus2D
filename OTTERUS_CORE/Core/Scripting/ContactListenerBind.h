#pragma once

#include <sol/sol.hpp>
#include <entt.hpp>
#include <physics/ContactListener.h>

namespace otterus_core::Scripting {

	class ContactListenerBinder
	{
	private:
		static std::tuple<sol::object, sol::object> GetUserData(otterus_physics::ContactListener& contactListener, sol::this_state s);
			
	public:
		static void CreateLuaContactListener(sol::state& lua, entt::registry& registry);


	};

}