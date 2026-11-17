#include "RigidBodyComponent.h"
#include <entt.hpp>

namespace otterus_core::ECS {
	std::string RigidBodyComponent::to_string() const
	{
		std::stringstream ss;
		ss <<
			"==== RigidBody Component ==== \n" <<
			"Velocity: [ x = " << velocity.x << ", y = " << velocity.y << "\n";

		return ss.str();
	}

	void RigidBodyComponent::CreateRigidBodyBind(sol::state& lua)
	{
		lua.new_usertype<RigidBodyComponent>(
			"RigidBody",
			"type_id", entt::type_hash<RigidBodyComponent>::value,
			sol::call_constructor,
			sol::factories(
				[](const glm::vec2& velocity) {
					return RigidBodyComponent{ .velocity = velocity };
				}
			),
			"velocity", &RigidBodyComponent::velocity,
			"to_string", &RigidBodyComponent::to_string
		);
	}
}