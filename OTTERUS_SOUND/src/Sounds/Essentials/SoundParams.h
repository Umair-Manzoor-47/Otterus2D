#pragma once
#include <string>

namespace otterus_sound {

	struct SoundParams
	{
		std::string name{ "" }, description{ "" }, filename{ "" };
		double duration{ 0.0 };
	};
}