#include "AnimationComponent.h"
#include <entt.hpp>

namespace otterus_core::ECS {
	std::string AnimationComponent::to_string() const
	{
		
		std::stringstream ss;
		ss <<
			"==== Animation Component ==== \n" << std::boolalpha <<
			"Num Frames: " << numFrames << "\n" <<
			"Frame Rate: " << frameRate << "\n" <<
			"Frame Offset: " << frameOffset << "\n" <<
			"bVertical: " << vertical << "\n" <<
			"bLooped: " << looped << "\n";

		return ss.str();
	}

	void otterus_core::ECS::AnimationComponent::CreateAnimationLuaBind(sol::state& lua)
	{

		lua.new_usertype<AnimationComponent>(
			"Animation",
			"type_id", &entt::type_hash<AnimationComponent>::value,
			sol::call_constructor,
			sol::factories(
				[](int numFrames = 1, int frameRate = 1, int frameOffset = 0, bool isVertical = false, bool looped = false) {
					return AnimationComponent{
						.numFrames = numFrames,
						.frameRate = frameRate,
						.frameOffset = frameOffset,
						.vertical = isVertical,
						.looped = looped
					};
				}
			),
			"num_frames", &AnimationComponent::numFrames,
			"frame_rate", &AnimationComponent::frameRate,
			"frame_Offset", &AnimationComponent::frameOffset,
			"current_frame", &AnimationComponent::currentFrame,
			"is_vertical", &AnimationComponent::vertical,
			"looped", &AnimationComponent::looped,
			"reset", [](AnimationComponent& anim) {
				anim.currentFrame = 0;
				anim.startTime = SDL_GetTicks();
			},
			"to_string", &AnimationComponent::to_string
		);

	}
}