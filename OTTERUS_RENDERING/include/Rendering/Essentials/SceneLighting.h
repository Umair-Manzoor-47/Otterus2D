#pragma once
#include "Vertex.h"

namespace otterus_rendering
{
	struct SceneLighting
	{
		Color ambientColor{ .r = 255, .g = 255, .b = 255, .a = 255 };
		bool enabled{ true };
	};
}
