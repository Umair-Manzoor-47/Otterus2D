#include "Texture.h"

namespace otterus_rendering {
	Texture::Texture() :
		Texture(0,0,0, TextureType::NONE)
	{}
	Texture::Texture(GLuint texID, int width, int height, TextureType type, const std::string& texturePath):
		m_texID{ texID }, m_width{ width }, m_height{ height }, m_type{ type }, m_path{texturePath}
	{}

	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_texID);
	}

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}


}

