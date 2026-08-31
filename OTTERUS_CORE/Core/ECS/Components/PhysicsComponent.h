#pragma once
#include <Physics/Box2Dwrappers.h>
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <entt.hpp>

namespace otterus_core::ECS {
	
	constexpr float METERS_TO_PIXELS = 12.f;
	constexpr float PIXELS_TO_METERS = 1.f / METERS_TO_PIXELS;

	enum class RigidbodyType {
		STATIC = 0,
		KINEMATIC,
		DYNAMIC
	};

	struct PhysicsAttributes
	{
		RigidbodyType type{RigidbodyType::STATIC};
		float density{ 1.f }, friction{ 0.2f }, restitution{ 0.2f };
		float restitutionThreshold{ 1.f }, radius{ 0.f }, gravityScale{ 1.f };

		glm::vec2 position{ 0.f }, scale{ 1.f }, boxSize{ 0.f }, offset{ 0.f };
		bool circle{ false }, boxShape{ true }, fixedRotation{ true };

		uint16_t filterCategory{ 0 }, filterMask{ 0 };
		int16_t groupIndex{ 0 };
	};


	class PhysicsComponent
	{
	private:
		otterus_physics::PhysicsWorld m_PhysicsWorld;
		std::shared_ptr<b2Body> m_RigidBody;

		PhysicsAttributes m_InitialAttribs;

	public:
		PhysicsComponent(otterus_physics::PhysicsWorld physicsWorld, const PhysicsAttributes& attribs);
		~PhysicsComponent() = default;

		void Init(int windowWidth, int windowHeight);
		b2Body* GetBody() { return m_RigidBody.get(); };

		static void CreatePhysicsLuaBind(sol::state& lua, entt::registry& registry);
	};

}