#pragma once
#include <sol/sol.hpp>

namespace otterus_core::ECS {
	struct AnimationComponent
	{
		int numFrames{ 1 }, frameRate{ 1 }, frameOffset{ 0 }, currentFrame{ 0 };
		bool vertical{ false };

		static void CreateAnimationLuaBind(sol::state& lua);
	};

}