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
		auto view = registry.view<PhysicsSystem, TransformComponent, PhysicsComponent>();

		auto scaleWidth = 640.f / METERS_TO_PIXELS;
		auto scaleHeight = 480.f / METERS_TO_PIXELS;
	}
}