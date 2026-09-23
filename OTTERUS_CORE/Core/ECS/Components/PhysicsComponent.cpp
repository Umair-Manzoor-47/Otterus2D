#include "PhysicsComponent.h"
#include "../../CoreUtilities/CoreEngineData.h"
#include <Logger/Logger.h>

using namespace otterus_physics;
namespace otterus_core::ECS {
	PhysicsComponent::PhysicsComponent()
		: PhysicsComponent(PhysicsAttributes{})
	{}
	PhysicsComponent::PhysicsComponent(const PhysicsAttributes& attribs)
		: m_RigidBody{ nullptr }, m_UserData{ nullptr }, m_InitialAttribs{ attribs }
	{}

	void PhysicsComponent::Init(PhysicsWorld physicsWorld, int windowWidth, int windowHeight)
	{
		auto PIXELS_TO_METERS = CoreEngineData::GetInstance().PixelsToMeters();
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
		m_RigidBody = MakeSharedBody(physicsWorld->CreateBody(&bodyDef));
	
		if (!m_RigidBody)
		{
			OTTERUS_ERROR("Failed to create rigidbody.");
			return;
		}

		// Shape Creation

		b2PolygonShape polygonShape;
		b2CircleShape circleShape;

		if(Circle){
			circleShape.m_radius = PIXELS_TO_METERS * m_InitialAttribs.radius * m_InitialAttribs.scale.x;
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
		// UserData
		m_UserData = std::make_shared<UserData>();
		m_UserData->userData = m_InitialAttribs.objectData;
		m_UserData->typeId = entt::type_hash<ObjectData>::value();

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
		fixtureDef.isSensor				= m_InitialAttribs.isSensor;
		fixtureDef.userData.pointer		= reinterpret_cast<uintptr_t>(m_UserData.get());
		
		auto fixutre = m_RigidBody->CreateFixture(&fixtureDef);
		
		if (!fixutre) {
			OTTERUS_ERROR("Failed to create fixture.");
			return;
		}
	}
	
	const bool PhysicsComponent::IsSensor() const
	{
		if (!m_RigidBody)
			return false;

		return m_RigidBody->GetFixtureList()->IsSensor();
	}

	void PhysicsComponent::CreatePhysicsLuaBind(sol::state& lua, entt::registry& registry)
	{
		lua.new_usertype<ObjectData>(
			"ObjectData",
			"type_id", &entt::type_hash<ObjectData>::value,
			sol::call_constructor,
			sol::factories(
				[](const std::string& tag, const std::string& group, bool isCollider, 
					bool isTrigger, const std::uint32_t entityID) 
				{
					return ObjectData{
						.tag		= tag,
						.group		= group,
						.isCollider   = isCollider,
						.isTrigger	= isTrigger,
						.entityId	= entityID
					};

				},
				[](const sol::table& params){
					return ObjectData{
						.tag		= params["tag"].get_or(std::string{""}),
						.group		= params["group"].get_or(std::string{""}),
						.isCollider   = params["isCollider"].get_or(false),
						.isTrigger	= params["isTrigger"].get_or(false),
						.entityId	= params["entityID"].get_or((std::uint32_t)entt::null)
					};
				}
			),
			"to_string", &ObjectData::to_string
		);

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
				},
				[](const sol::table& physicsAttribs) {
					return PhysicsAttributes{
						.type					= physicsAttribs["type"].get_or(RigidbodyType::STATIC),
						.density				= physicsAttribs["density"].get_or(1.f),
						.friction				= physicsAttribs["friction"].get_or(0.2f),
						.restitution			= physicsAttribs["restitution"].get_or(0.2f),
						.restitutionThreshold	= physicsAttribs["restitutionThreshold"].get_or(1.f),
						.radius					= physicsAttribs["radius"].get_or(0.f),
						.gravityScale			= physicsAttribs["gravityScale"].get_or(1.f),
						.position				= glm::vec2{ physicsAttribs["position"]["x"].get_or(0.f), physicsAttribs["position"]["y"].get_or(0.f) },
						.scale					= glm::vec2{ physicsAttribs["scale"]["x"].get_or(1.f), physicsAttribs["scale"]["y"].get_or(1.f) },
						.boxSize				= glm::vec2{ physicsAttribs["boxSize"]["x"].get_or(0.f), physicsAttribs["boxSize"]["y"].get_or(0.f) },
						.offset					= glm::vec2{ physicsAttribs["offset"]["x"].get_or(0.f), physicsAttribs["offset"]["y"].get_or(0.f) },
						.circle					= physicsAttribs["circle"].get_or(false),
						.boxShape				= physicsAttribs["boxShape"].get_or(true),
						.fixedRotation			= physicsAttribs["fixedRotation"].get_or(true),
						.isSensor				= physicsAttribs["isSensor"].get_or(false),
						.filterCategory			= physicsAttribs["filterCategory"].get_or((uint16_t)0),
						.filterMask				= physicsAttribs["filterMask"].get_or((uint16_t)0),
						.groupIndex				= physicsAttribs["groupIndex"].get_or((int16_t)0),
						.objectData				= new ObjectData{
							.tag		= physicsAttribs["ObjectData"]["tag"].get_or(std::string{""}),
							.group		= physicsAttribs["ObjectData"]["group"].get_or(std::string{""}),
							.isCollider	= physicsAttribs["ObjectData"]["isCollider"].get_or(false),
							.isTrigger	= physicsAttribs["ObjectData"]["isTrigger"].get_or(false),
							.entityId	= physicsAttribs["ObjectData"]["entityID"].get_or((std::uint32_t)entt::null)
						}
					};
				}
			),
			"type", &PhysicsAttributes::type,
			"density", &PhysicsAttributes::density,
			"friction", &PhysicsAttributes::friction,
			"restitution", &PhysicsAttributes::restitution,
			"restitutionThreshold", &PhysicsAttributes::restitutionThreshold,
			"radius", &PhysicsAttributes::radius,
			"gravityScale", &PhysicsAttributes::gravityScale,
			"position", &PhysicsAttributes::position,
			"scale", &PhysicsAttributes::scale,
			"boxSize", &PhysicsAttributes::boxSize,
			"offset", &PhysicsAttributes::offset,
			"circle", &PhysicsAttributes::circle,
			"boxShape", &PhysicsAttributes::boxShape,
			"fixedRotation", &PhysicsAttributes::fixedRotation,
			"isSensor", &PhysicsAttributes::isSensor,
			"filterCategory", &PhysicsAttributes::filterCategory,
			"filterMask", &PhysicsAttributes::filterMask,
			"groupIndex", &PhysicsAttributes::groupIndex,
			"objectData", &PhysicsAttributes::objectData
		);

		auto& physicsWorld = registry.ctx().get<PhysicsWorld>();

		if (!physicsWorld)
		{
			return;
		}

		// THIS is BLUEPRINT that can be expanded as per functionality needed from Box2D

		lua.new_usertype<PhysicsComponent>(
			"PhysicsComponent",
			"type_id", &entt::type_hash<PhysicsComponent>::value,
			sol::call_constructor,
			sol::factories(
				[&](const PhysicsAttributes& attribs) {
					PhysicsComponent pc{ attribs };
					pc.Init(physicsWorld, 640, 480); // TODO: Add real window values that could be dynamic and responsive
					return pc;
				}
			),
			"linear_impulse", [](PhysicsComponent& pc, const glm::vec2& impulse) {
				
				auto body = pc.GetBody();

				if (!body) {
					return;
				}
				body->ApplyLinearImpulse(b2Vec2{ impulse.x, impulse.y }, body->GetPosition(), true);

			},
			"angular_impulse", [](PhysicsComponent& pc, float impulse) {
				
				auto body = pc.GetBody();

				if (!body) {
					return;
				}

				body->ApplyAngularImpulse(impulse, true);

			},
			"set_linear_velocity", [](PhysicsComponent& pc, const glm::vec2 velocity) {

				auto body = pc.GetBody();

				if (!body) {
					return;
				}

				body->SetLinearVelocity(b2Vec2{velocity.x, velocity.y});

			},
			"get_linear_velocity", [](PhysicsComponent& pc) {

				auto body = pc.GetBody();

				if (!body) {
					return glm::vec2{0.f};
				}

				const auto& linearVelocity = body->GetLinearVelocity();
				return glm::vec2{linearVelocity.x, linearVelocity.y};
			},
			"set_angular_velocity", [](PhysicsComponent& pc, float angularVelocity) {

				auto body = pc.GetBody();

				if (!body) {
					return;
				}

				body->SetAngularVelocity(angularVelocity);

			},
			"get_angular_velocity", [](PhysicsComponent& pc) {

				auto body = pc.GetBody();

				if (!body) {
					return 0.f;
				}

				return body->GetAngularVelocity();
			},
			"set_gravity_scale", [](PhysicsComponent& pc, float gravityScale) {
				auto body = pc.GetBody();

				if (!body) {
					return;
				}
				body->SetGravityScale(gravityScale);
			}
		);

	}
}