#pragma once
#include <sol/sol.hpp>
#include <SDL.h>

namespace otterus_core::ECS {
	struct AnimationComponent
	{
		int numFrames{ 1 }, frameRate{ 1 }, frameOffset{ 0 }, currentFrame{ 0 };
		int startTime{ static_cast<int>(SDL_GetTicks()) };
		bool vertical{ false }, looped{ false };

		static void CreateAnimationLuaBind(sol::state& lua);
	};

}