#pragma once

#include <any>
#include <string>
#include <sstream>


namespace otterus_physics {

	struct UserData
	{
		std::any userData{};
		std::uint32_t typeId{0}; // type_hash for entt::meta

	};

	struct ObjectData
	{
		std::string tag{ "" }, group{ "" };
		bool collider{ false }, trigger{ false };
		std::uint32_t entityId{ 0 };

		[[nodiscard]] std::string to_string() const
		{
			std::stringstream ss;
			ss <<
				"==== Object Data ==== \n" << std::boolalpha <<
				"Tag: " << tag << "\n" <<
				"Group: " << group << "\n" <<
				"isCollider: " << collider << "\n" <<
				"isTrigger: " << trigger << "\n" <<
				"EntityID: " << entityId << "\n";

			return ss.str();

		}
	};
}