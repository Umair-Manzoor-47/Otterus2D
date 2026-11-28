#pragma once
#include <Physics/Box2Dwrappers.h>
#include <Physics/UserData.h>
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <entt.hpp>
#include "Physics/PhysicsUtilities.h"

namespace otterus_core::ECS {

	struct PhysicsAttributes
	{
		otterus_physics::RigidBodyType type{otterus_physics::RigidBodyType::STATIC};
		float density{ 1.f }, friction{ 0.2f }, restitution{ 0.2f };
		float restitutionThreshold{ 1.f }, radius{ 0.f }, gravityScale{ 1.f };

		glm::vec2 position{ 0.f }, scale{ 1.f }, boxSize{ 0.f }, offset{ 0.f };
		bool circle{ false }, boxShape{ true }, fixedRotation{ true }, isSensor{ false };
		bool isBullet{ false };

		/*
		 * Filters - This is used to filter collisions on shapes.
		 * It affects shape-vs-shape collision and shape-versus-query collision (such as b2World_CastRay).
		 * See https://box2d.org/documentation/group__shape.html#structb2_filter for more information.
		 */
		uint16_t filterCategory{ 0 };
		uint16_t filterMask{ 0 };
		int16_t groupIndex{ 0 };

		/* This is the user specified data for each body. */
		otterus_physics::ObjectData objectData{};
	};


	class PhysicsComponent
	{
	private:
		std::shared_ptr<otterus_physics::UserData> m_UserData;
		std::shared_ptr<b2Body> m_RigidBody;
		PhysicsAttributes m_InitialAttribs;

	public:
		PhysicsComponent();
		PhysicsComponent(const PhysicsAttributes& attribs);
		~PhysicsComponent() = default;

		void Init(otterus_physics::PhysicsWorld physicsWorld, int windowWidth, int windowHeight);
		b2Body* GetBody() { return m_RigidBody.get(); };

		/* The attributes may have changed.
		 * we need to make a function that will refill the attributes */
		const PhysicsAttributes& GetAttributes() const { return m_InitialAttribs; }
		PhysicsAttributes& GetChangableAttributes() { return m_InitialAttribs; }
		otterus_physics::UserData* GetUserData() { return m_UserData.get(); };
		const bool IsSensor() const;

		static void CreatePhysicsLuaBind(sol::state& lua, entt::registry& registry);
	};

}
