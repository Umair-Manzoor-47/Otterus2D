#include "AnimationComponent.h"
#include <entt.hpp>

void otterus_core::ECS::AnimationComponent::CreateAnimationLuaBind(sol::state& lua)
{

	lua.new_usertype<AnimationComponent>(
		"Animation",
		"type_id", &entt::type_hash<AnimationComponent>::value, 
		sol::call_constructor,
		sol::factories(
			[](int numFrames =  1 , int frameRate = 1, int frameOffset = 0 , bool isVertical = false) {
				return AnimationComponent{
					.numFrames = numFrames,
					.frameRate = frameRate,
					.frameOffset = frameOffset,
					.vertical = isVertical
				};
			}
		),
		"num_frames", &AnimationComponent::numFrames,
		"frame_rate", &AnimationComponent::frameRate,
		"frame_Offset", &AnimationComponent::frameOffset,
		"current_frame", &AnimationComponent::currentFrame,
		"is_vertical", &AnimationComponent::vertical
	);

}
