#pragma once

namespace otterus_physics
{
	enum class RigidBodyType {
		STATIC = 0,
		KINEMATIC,
		DYNAMIC
	};

/*
 * The collision category bits.
 * Normally you would just set one bit.
 * The category bits should represent object types.
 */
enum class FilterCategory : std::uint16_t
{
	// Default
	NO_CATEGORY = 0,
	PLAYER = 1 << 0,
	ENEMY = 1 << 1,
	ITEM = 1 << 2,
	WALLS = 1 << 3,
	GROUND = 1 << 4,
	TRIGGER = 1 << 5,
	PROJECTILE = 1 << 6,
	// User defined object types with custom names but
	// in the backend, the original names and values are used.
	CATEGORY_7 = 1 << 7,
	CATEGORY_8 = 1 << 8,
	CATEGORY_9 = 1 << 9,
	CATEGORY_10 = 1 << 10,
	CATEGORY_11 = 1 << 11,
	CATEGORY_12 = 1 << 12,
	CATEGORY_13 = 1 << 13,
	CATEGORY_14 = 1 << 14,
	CATEGORY_15 = 1 << 15
};

}