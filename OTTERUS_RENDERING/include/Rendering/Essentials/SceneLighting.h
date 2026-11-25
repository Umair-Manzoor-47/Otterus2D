#pragma once
#include <glm/glm.hpp>

namespace otterus_rendering
{
	struct SceneLighting
	{
		glm::vec4 ambientColor{ 1.0f, 1.0f, 1.0f, 1.0f };
		bool enabled{ true };
	};
}
