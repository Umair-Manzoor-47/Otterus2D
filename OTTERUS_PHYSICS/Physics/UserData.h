#pragma once

#include <any>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>

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
		std::vector<ObjectData> contactEntities;

		friend bool operator==( const ObjectData& a, const ObjectData& b );
		bool AddContact(const ObjectData& objectData);
		bool RemoveContact(const ObjectData& objectData);

		[[nodiscard]] std::string to_string() const;
	};
}