#include "PhysicsSystem.h"
#include "../ECS/Components/BoxColliderComponent.h"
#include "../ECS/Components/CircleColliderComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include "../ECS/Components/PhysicsComponent.h"
#include <Logger/Logger.h>

using namespace otterus_core::ECS;

namespace otterus_core::Systems {
	PhysicsSystem::PhysicsSystem(otterus_core::ECS::Registry& registry)
		: m_registry{registry}
	{}

	void PhysicsSystem::Update(entt::registry& registry)
	{
		auto boxView = registry.view<PhysicsComponent, TransformComponent, BoxColliderComponent>();

		auto scaledWidth = 640.f / METERS_TO_PIXELS;
		auto scaledHeight = 480.f / METERS_TO_PIXELS;
	
		for (auto entity: boxView)
		{
			auto& physics = boxView.get<PhysicsComponent>(entity);
			auto rigidBody = physics.GetBody();

			if (!rigidBody) 
				continue;

			auto& transform = boxView.get<TransformComponent>(entity);
			auto& boxCollider = boxView.get<BoxColliderComponent>(entity);

			const auto& bodyPosition = rigidBody->GetPosition();

			transform.position.x = ((scaledWidth / 2.f) + bodyPosition.x) * METERS_TO_PIXELS -
				(boxCollider.width * transform.scale.x) / 2.f - boxCollider.offset.x;
			transform.position.y = ((scaledHeight / 2.f) + bodyPosition.y) * METERS_TO_PIXELS -
				(boxCollider.height * transform.scale.y) / 2.f - boxCollider.offset.y;

		
			if (!rigidBody->IsFixedRotation())
			{
				transform.rotation = glm::degrees(rigidBody->GetAngle());
			}
		}

		auto circleView = registry.view<PhysicsComponent, TransformComponent, CircleColliderComponent>();

		for (auto entity : circleView)
		{
			auto& physics = circleView.get<PhysicsComponent>(entity);
			auto rigidBody = physics.GetBody();

			if (!rigidBody)
				continue;

			auto& transform = circleView.get<TransformComponent>(entity);
			auto& circleCollider = circleView.get<CircleColliderComponent>(entity);

			const auto& bodyPosition = rigidBody->GetPosition();

			transform.position.x = ((scaledWidth / 2.f) + bodyPosition.x) * METERS_TO_PIXELS -
				(circleCollider.radius * transform.scale.x) - circleCollider.offset.x;
			transform.position.y = ((scaledHeight / 2.f) + bodyPosition.y) * METERS_TO_PIXELS -
				(circleCollider.radius * transform.scale.y)- circleCollider.offset.y;


			if (!rigidBody->IsFixedRotation())
			{
				transform.rotation = glm::degrees(rigidBody->GetAngle());
			}
		}
	}
}