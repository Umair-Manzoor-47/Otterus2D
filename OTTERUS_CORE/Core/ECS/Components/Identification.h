#pragma once
#include <string>

namespace otterus_core::ECS {

	struct Identification
	{
		std::string name{ "GameObject" }, group{ "" };
		int32_t entity_id{ -1 };
	};


}