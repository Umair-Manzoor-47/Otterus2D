#pragma once
#include <sol/sol.hpp>
#include <glm/glm.hpp>

namespace otterus_core::ECS {

	struct BoxColliderComponent
	{
		int width{ 1 }, height{ 1 };
		glm::vec2 offset{ glm::vec2{0} };
		bool Colliding{ false };

		static void CreateBoxColliderLuaBind(sol::state& lua);
	};


}