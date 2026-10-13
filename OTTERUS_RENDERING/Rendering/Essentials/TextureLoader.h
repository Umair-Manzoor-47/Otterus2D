#pragma once
#include "Texture.h"
#include <memory>
#include <SOIL/SOIL.h>
#include <glad/glad.h>
#include <string>
#include <Logger/Logger.h>

namespace otterus_rendering {

	class TextureLoader
	{
	private:
		static GLuint CreateTexture();
		static void SetTextureParams(bool blended);
		static bool UploadToGPU(unsigned char* image, int width, int height, int channels);
		static bool LoadFBTexture(GLuint& id, int& width, int& height);
		static bool LoadTextureFromMemory(const unsigned char* imageData, size_t length, GLuint& id, int& width, int& height, bool blended = false);

	public:
		TextureLoader() = delete;
		static std::shared_ptr<Texture> Create(const std::string& path, Texture::TextureType type);
		static std::shared_ptr<Texture> Create(const Texture::TextureType type, int width, int height);
		static std::shared_ptr<Texture> CreateFromMemory(const unsigned char* imageData, size_t length, bool blended = false);

	};
}