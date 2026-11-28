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

	/*
	 * ObjectData
	 * Currently this struct is used for all Rigidbodies in Otterus2D.
	 * This is what the user data is set to and checked for in the contact listener bindings.
	 * You may need a specific user data setup for your own specific needs; however,
	 * you can always use the tag and group to do different functions on the body as needed.
	 *
	 * For instance, in the lua scripts:
	 *
	 *
		function RunSpikeDeath(objectA, objectB)
			-- TODO: Kill the player
			-- TODO: Add blood to the spikes
		end

		function UpdateContacts()
			local uda, udb = ContactListener.get_user_data()
			if uda and udb then
				if uda.tag == "player" and udb.group == "spikes" then
					RunSpikeDeath(uda, udb)
				end
			end
		end
	 */
	struct ObjectData
	{
		std::string tag{ "" }, group{ "" };
		bool isCollider{ false }, isTrigger{ false }, isFriendly{ false };;
		std::uint32_t entityId{ 0 };
		std::vector<ObjectData> contactEntities;

		friend bool operator==( const ObjectData& a, const ObjectData& b );
		bool AddContact(const ObjectData& objectData);
		bool RemoveContact(const ObjectData& objectData);

		[[nodiscard]] std::string to_string() const;
	};
}