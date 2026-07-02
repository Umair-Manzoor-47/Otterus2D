#pragma once
#include <glad/glad.h>
#include <string>

namespace otterus_rendering {

	class Texture {
	
	public: enum class TextureType { PIXEL = 0, BLENDED, NONE };

	private:
		GLuint m_texID;
		int m_width, m_height;
		std::string m_path;
		TextureType m_type;


	public:
		Texture();
		Texture(GLuint texID, int width, int height, TextureType type = TextureType::PIXEL, const std::string& texturePath = "");
		~Texture() = default;

		inline const GLuint GetID() const { return m_texID; }
		inline const int GetWidth() const { return m_width; }
		inline const int GetHeight() const { return m_height; }

		void Bind();
		void Unbind();
	};

}