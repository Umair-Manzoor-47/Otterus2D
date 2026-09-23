#pragma once
#include <Physics/Box2Dwrappers.h>
#include <Physics/UserData.h>
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <entt.hpp>

namespace otterus_core::ECS {
	
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
		bool circle{ false }, boxShape{ true }, fixedRotation{ true }, isSensor{ false };

		uint16_t filterCategory{ 0 }, filterMask{ 0 };
		int16_t groupIndex{ 0 };

		otterus_physics::ObjectData* objectData{};
	};


	class PhysicsComponent
	{
	private:
		std::shared_ptr<b2Body> m_RigidBody;
		std::shared_ptr<otterus_physics::UserData> m_UserData;
		PhysicsAttributes m_InitialAttribs;

	public:
		PhysicsComponent();
		PhysicsComponent(const PhysicsAttributes& attribs);
		~PhysicsComponent() = default;

		void Init(otterus_physics::PhysicsWorld physicsWorld, int windowWidth, int windowHeight);
		b2Body* GetBody() { return m_RigidBody.get(); };
		otterus_physics::UserData* GetUserData() { return m_UserData.get(); };
		const bool IsSensor() const;

		static void CreatePhysicsLuaBind(sol::state& lua, entt::registry& registry);
	};

}