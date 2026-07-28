#pragma once
#include <sol/sol.hpp>
#include <glm/glm.hpp>

namespace otterus_core::ECS {

	struct CircleColliderComponent
	{
		float radius{ 0 };
		glm::vec2 offset{ glm::vec2{0} };
		bool colliding{ false };

		static void CreateLuaCircleColliderBind(sol::state& lua);
	};


}