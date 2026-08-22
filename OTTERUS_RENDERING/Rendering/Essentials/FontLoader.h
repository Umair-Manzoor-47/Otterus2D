#pragma once

#include <memory>
#include <string>

namespace otterus_rendering {

	class FontLoader
	{
	public:
		FontLoader() = delete;

		static std::shared_ptr<class Font> Create(const std::string& fontPath, float fontSize = 32.f, int width = 512, int height = 512);
		static std::shared_ptr<class Font> CreateFromMemory(const unsigned char* fontPath, float fontSize = 32.f, int width = 512, int height = 512);




	};
}