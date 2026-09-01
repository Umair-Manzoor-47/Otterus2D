#include "PhysicsComponent.h"
#include <Logger/Logger.h>

namespace otterus_core::ECS {
	PhysicsComponent::PhysicsComponent(otterus_physics::PhysicsWorld physicsWorld, const PhysicsAttributes& attribs)
		: m_PhysicsWorld{ physicsWorld}, m_RigidBody { nullptr }, m_InitialAttribs { attribs }
	{}

	void PhysicsComponent::Init(int windowWidth, int windowHeight)
	{
		if (!m_PhysicsWorld) {
			OTTERUS_ERROR("Failed to create Physics component -- Physics world is nullptr");
			return;
		}

		bool Circle(m_InitialAttribs.circle);
		
		// body def
		b2BodyDef bodyDef{};
		bodyDef.type = static_cast<b2BodyType>(m_InitialAttribs.type);

		// Initial Position
		bodyDef.position.Set(
			(m_InitialAttribs.position.x + m_InitialAttribs.offset.x - (windowWidth * 0.5f ) + 
				((Circle ? m_InitialAttribs.radius: m_InitialAttribs.boxSize.x) * m_InitialAttribs.scale.x) * 0.5f) * PIXELS_TO_METERS,
			(m_InitialAttribs.position.y + m_InitialAttribs.offset.y - (windowHeight * 0.5f) +
				((Circle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.y) * m_InitialAttribs.scale.y) * 0.5f) * PIXELS_TO_METERS
		
		);

		bodyDef.gravityScale = m_InitialAttribs.gravityScale;
		bodyDef.fixedRotation = m_InitialAttribs.fixedRotation;

		// Rigidbody Creation
		m_RigidBody = otterus_physics::MakeSharedBody(m_PhysicsWorld->CreateBody(&bodyDef));
	
		if (!m_RigidBody)
		{
			OTTERUS_ERROR("Failed to create rigidbody.");
			return;
		}

		// Shape Creation

		b2PolygonShape polygonShape;
		b2CircleShape circleShape;

		if(Circle){
			circleShape.m_radius = m_InitialAttribs.radius;
		}
		else if (m_InitialAttribs.boxShape)
		{
			polygonShape.SetAsBox(
				PIXELS_TO_METERS * m_InitialAttribs.boxSize.x * m_InitialAttribs.scale.x * 0.5f,
				PIXELS_TO_METERS * m_InitialAttribs.boxSize.y * m_InitialAttribs.scale.y * 0.5f
			);
		}
		else
		{
			// TODO: Create Polygon shape
		}

		// Fixture Defs
		b2FixtureDef fixtureDef{};
		if (Circle)
		{
			fixtureDef.shape = &circleShape;

		}
		else
		{
			fixtureDef.shape = &polygonShape;
		}

		fixtureDef.density				= m_InitialAttribs.density;
		fixtureDef.friction				= m_InitialAttribs.friction;
		fixtureDef.restitution			= m_InitialAttribs.restitution;
		fixtureDef.restitutionThreshold = m_InitialAttribs.restitutionThreshold;
		
		auto fixutre = m_RigidBody->CreateFixture(&fixtureDef);
		
		if (!fixutre) {
			OTTERUS_ERROR("Failed to create fixture.");
			return;
		}
	}

	void PhysicsComponent::CreatePhysicsLuaBind(sol::state& lua, entt::registry& registry)
	{
		// TODO: CREATE BINDINGS
		// 1. Physics Attributes		
		// 2. Bind Component


	}
}