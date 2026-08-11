#pragma once
#include <string>

namespace otterus_sounds {

	struct SoundParams
	{
		std::string name{ "" }, description{ "" }, filename{ "" };
		double duration{ 0.0 };
	};
}