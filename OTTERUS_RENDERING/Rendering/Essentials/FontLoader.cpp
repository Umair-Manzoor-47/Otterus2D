#include "FontLoader.h"
#include "Font.h"

namespace otterus_rendering {

    std::shared_ptr<Font> FontLoader::Create(const std::string& fontPath, float fontSize, int width, int heigt)
    {
        return std::shared_ptr<Font>();
    }

    std::shared_ptr<Font> FontLoader::CreateFromMemory(const unsigned char* fontPath, float fontSize, int width, int heigt)
    {
        return std::shared_ptr<Font>();
    }
}