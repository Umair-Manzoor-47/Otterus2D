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
		bool isCollider{ false }, isTrigger{ false };
		std::uint32_t entityId{ 0 };

		[[nodiscard]] std::string to_string() const
		{
			std::stringstream ss;
			ss <<
				"==== Object Data ==== \n" << std::boolalpha <<
				"Tag: " << tag << "\n" <<
				"Group: " << group << "\n" <<
				"isCollider: " << isCollider << "\n" <<
				"isTrigger: " << isTrigger << "\n" <<
				"EntityID: " << entityId << "\n";

			return ss.str();

		}
	};
}