#pragma once
#include "../ECS/Registry.h"

namespace otterus_core::Systems {

	class AnimationSystem
	{
	private:
		otterus_core::ECS::Registry& m_registry;
	public:
		AnimationSystem(otterus_core::ECS::Registry& registry);
		~AnimationSystem() = default;

		void Update();
	};




}