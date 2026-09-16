#include "TextureLoader.h"

namespace otterus_rendering {
	
	GLuint TextureLoader::CreateTexture()
	{
		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		return texID;
	}
	
	void TextureLoader::SetTextureParams(bool blended)
	{

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (!blended) {

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
	
	}

	bool TextureLoader::UploadToGPU(unsigned char* image, int width, int height, int channels)
	{
		GLint format = GL_RGBA;

		switch (channels) {
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		}
		glTexImage2D(
			GL_TEXTURE_2D,                   // target              -- Specifies the target texture
			0,								 // level               -- Level of detail, 0 is base image level
			format,							 // internal format     -- Number of color components
			width,							 // width               -- Width of image
			height,                          // height              -- Height of image
			0,								 // border
			format,							 // format              -- Format of the pixel data
			GL_UNSIGNED_BYTE,				 // type			    -- Type of pixel data
			image							 // data
		);

		// once texture is created, we can free SOIL image data
		SOIL_free_image_data(image);
		return true;
	}

	bool TextureLoader::LoadFBTexture(GLuint& id, int& width, int& height)
	{
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


		return true;
	}

	std::shared_ptr<Texture> otterus_rendering::TextureLoader::Create(const std::string& path, Texture::TextureType type)
	{
		int channels = 0, width{ 0 }, height{ 0 };
		unsigned char* image = SOIL_load_image(
			path.c_str(),                              // filepath         -- Path to texture file including name
			&width,									   // width            -- Width of image
			&height,								   // height           -- Height of image
			&channels,                                 // channels         -- Number of channels
			SOIL_LOAD_AUTO                             // force channels   -- Force the channel count
		);

		if (!image) {

			OTTERUS_ERROR("Failed to load image from [{0}] -- {1}", path, SOIL_last_result());
			//OTTERUS_ERROR("Failed to load image from {0}", path);
			return nullptr;
		}

		GLuint texID = CreateTexture();

		switch (type) {
		case Texture::TextureType::PIXEL:
			SetTextureParams(false);
			break;
		case Texture::TextureType::BLENDED:
			SetTextureParams(true);
			break;
		
		// TODO: Add more texture type as needed, EX: framebuffer texture
		default:
			assert(false && "Current texture type is not defined, Please use a defined texture type.");
			return nullptr;
		}
		//SetTextureParams(blended);
		
		
		UploadToGPU(image, width, height, channels);

		return std::make_shared<Texture>(texID, width, height, type, path);
	}

	std::shared_ptr<Texture> TextureLoader::Create(const Texture::TextureType type, int width, int height)
	{
		OTTERUS_ASSERT(type == Texture::TextureType::FRAMEBUFFER && "Must be FrameBuffer type.");

		if (type != Texture::TextureType::FRAMEBUFFER)
		{
			OTTERUS_ERROR("Failed to create FBO Texture, incorrect TextureType passed in.");
			return nullptr;
		}
		GLuint id;
		glGenTextures(1, &id);
		LoadFBTexture(id, width, height);

		return std::make_shared<Texture>(id, width, height, type);
	}

}

