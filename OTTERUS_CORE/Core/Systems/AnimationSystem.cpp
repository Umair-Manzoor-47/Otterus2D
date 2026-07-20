#include "AnimationSystem.h"
#include "../ECS/Components/AnimationComponent.h"
#include "../ECS/Components/SpriteComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include <SDL.h>


using namespace otterus_core::ECS;
namespace otterus_core::Systems {
	AnimationSystem::AnimationSystem(otterus_core::ECS::Registry & registry)
		: m_registry(registry)
	{}

	void AnimationSystem::Update()
	{
		auto view = m_registry.GetRegistry().view<AnimationComponent, SpriteComponent, TransformComponent>();

		for (auto entity : view)
		{
			const auto& transform = view.get<TransformComponent>(entity);
			auto& sprite = view.get<SpriteComponent>(entity);
			auto& animation = view.get<AnimationComponent>(entity);

			animation.currentFrame = (SDL_GetTicks() * animation.frameRate / 1000) % animation.numFrames;

			if (animation.vertical)
			{
				sprite.uvs.v = animation.currentFrame * sprite.uvs.uv_height;
				sprite.uvs.u = animation.frameOffset * sprite.uvs.uv_width;
			}
			else
			{
				sprite.uvs.u = (animation.currentFrame * sprite.uvs.uv_width) + (animation.frameOffset * sprite.uvs.uv_width);

			}
		}
	}
}
