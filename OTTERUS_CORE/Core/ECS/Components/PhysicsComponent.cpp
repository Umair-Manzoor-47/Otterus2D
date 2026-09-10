#include "PhysicsComponent.h"
#include <Logger/Logger.h>

namespace otterus_core::ECS {
	PhysicsComponent::PhysicsComponent()
		: PhysicsComponent(PhysicsAttributes{})
	{}
	PhysicsComponent::PhysicsComponent(const PhysicsAttributes& attribs)
		: m_RigidBody { nullptr }, m_InitialAttribs { attribs }
	{}

	void PhysicsComponent::Init(otterus_physics::PhysicsWorld physicsWorld, int windowWidth, int windowHeight)
	{
		if (!physicsWorld) {
			OTTERUS_ERROR("Failed to create Physics component -- Physics world is nullptr");
			return;
		}

		bool Circle(m_InitialAttribs.circle);
		
		// body def
		b2BodyDef bodyDef{};
		bodyDef.type = static_cast<b2BodyType>(m_InitialAttribs.type);

		// Initial Position
		// transform.position is the top-left of the bounding box; the Box2D body
		// origin is its center. Half the bounding box is radius for a circle
		// (bounding box is 2*radius wide) and boxSize * 0.5 for a box.
		const float halfExtentX = Circle
			? m_InitialAttribs.radius * m_InitialAttribs.scale.x
			: m_InitialAttribs.boxSize.x * m_InitialAttribs.scale.x * 0.5f;
		const float halfExtentY = Circle
			? m_InitialAttribs.radius * m_InitialAttribs.scale.y
			: m_InitialAttribs.boxSize.y * m_InitialAttribs.scale.y * 0.5f;

		bodyDef.position.Set(
			(m_InitialAttribs.position.x + m_InitialAttribs.offset.x - (windowWidth * 0.5f) + halfExtentX) * PIXELS_TO_METERS,
			(m_InitialAttribs.position.y + m_InitialAttribs.offset.y - (windowHeight * 0.5f) + halfExtentY) * PIXELS_TO_METERS
		);

		bodyDef.gravityScale = m_InitialAttribs.gravityScale;
		bodyDef.fixedRotation = m_InitialAttribs.fixedRotation;

		// Rigidbody Creation
		m_RigidBody = otterus_physics::MakeSharedBody(physicsWorld->CreateBody(&bodyDef));
	
		if (!m_RigidBody)
		{
			OTTERUS_ERROR("Failed to create rigidbody.");
			return;
		}

		// Shape Creation

		b2PolygonShape polygonShape;
		b2CircleShape circleShape;

		if(Circle){
			circleShape.m_radius = PIXELS_TO_METERS * m_InitialAttribs.radius * (m_InitialAttribs.scale.x > m_InitialAttribs.scale.y ? m_InitialAttribs.scale.x : m_InitialAttribs.scale.y);
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


		lua.new_enum<RigidbodyType>(
			"BodyType", {
				{ "STATIC", RigidbodyType::STATIC },
				{ "KINEMATIC", RigidbodyType::KINEMATIC },
				{ "DYNAMIC", RigidbodyType::DYNAMIC }
			}
		);
		
		lua.new_usertype<PhysicsAttributes>(
			"PhysicsAttributes",
			sol::call_constructor,
			sol::factories(
				[] {
					return PhysicsAttributes{};
				}
				// TODO: More specific ctors
			),
			"type", &PhysicsAttributes::type,
			"density", &PhysicsAttributes::density,
			"friction", &PhysicsAttributes::friction,
			"restitution", &PhysicsAttributes::restitution,
			"restitution_threshold", &PhysicsAttributes::restitutionThreshold,
			"radius", &PhysicsAttributes::radius,
			"gravity_scale", &PhysicsAttributes::gravityScale,
			"position", &PhysicsAttributes::position,
			"scale", &PhysicsAttributes::scale,
			"box_size", &PhysicsAttributes::boxSize,
			"offset", &PhysicsAttributes::offset,
			"circle", &PhysicsAttributes::circle,
			"box_shape", &PhysicsAttributes::boxShape,
			"fixed_rotation", &PhysicsAttributes::fixedRotation,
			"filter_category", &PhysicsAttributes::filterCategory,
			"filter_mask", &PhysicsAttributes::filterMask,
			"group_index", &PhysicsAttributes::groupIndex
		);

		auto& physicsWorld = registry.ctx().get<otterus_physics::PhysicsWorld>();

		if (!physicsWorld)
		{
			return;
		}

		// THIS is BLUEPRINT that can be expanded as per functionality needed from Box2D

		lua.new_usertype<PhysicsComponent>(
			"PhysicsComp",
			"type_id", &entt::type_hash<PhysicsComponent>::value,
			sol::call_constructor,
			sol::factories(
				[&](const PhysicsAttributes& attribs) {
					PhysicsComponent pc{ attribs };
					pc.Init(physicsWorld, 640, 480); // TODO: Add real window values that could be dynamic and responsive
					return pc;
				}
			),
			"linear_impulse", sol::overload(
				[](PhysicsComponent& pc, const glm::vec2& impulse) {
					auto body = pc.GetBody();

					if (!body) {
						return;
					}
					body->ApplyLinearImpulse(b2Vec2{ impulse.x, impulse.y }, body->GetPosition(), true);

				}
			)
		);

	}
}