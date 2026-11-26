#pragma once

namespace otterus_core::ECS
{
	class Registry;
}

namespace otterus_core::Systems {

	class AnimationSystem
	{
	public:
		AnimationSystem();
		~AnimationSystem() = default;

		void Update(ECS::Registry& registry);
	};




}