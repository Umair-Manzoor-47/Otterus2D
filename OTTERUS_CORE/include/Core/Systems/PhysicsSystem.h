#pragma once

namespace otterus_core::ECS{
	class Registry;
}

namespace otterus_core::Systems {

	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem() = default;

		void Update(ECS::Registry& registry);
	};

}