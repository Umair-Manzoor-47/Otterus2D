#pragma once
#include "../ECS/Registry.h"

namespace otterus_core::Systems {

	class PhysicsSystem
	{
	private:
		otterus_core::ECS::Registry& m_registry;
	public:
		PhysicsSystem(otterus_core::ECS::Registry& registry);
		~PhysicsSystem() = default;

		void Update(entt::registry& registry);
	};

}