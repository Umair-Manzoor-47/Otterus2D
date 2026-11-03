#pragma once
#include <glad/glad.h>
#include <string>

namespace otterus_rendering {

	class Texture {
	
	public: enum class TextureType { PIXEL = 0, BLENDED, FRAMEBUFFER, NONE };

	private:
		GLuint m_texID;
		int m_width, m_height;
		std::string m_path;
		TextureType m_type;
		bool m_Tileset, m_EditorTexture;


	public:
		Texture();
		Texture(GLuint texID, int width, int height, TextureType type = TextureType::PIXEL,
			const std::string& texturePath = "", bool tileset = false);
		~Texture() = default;

		inline const GLuint GetID() const { return m_texID; }
		inline const int GetWidth() const { return m_width; }
		inline const int GetHeight() const { return m_height; }
		inline const bool IsTileset() const { return m_Tileset; }
		inline const bool IsEditorTexture() const { return m_EditorTexture; }

		inline const void SetIsTileset(bool isTileset) { m_Tileset = isTileset; }
		inline const void SetEditorTexture(bool isEditorTexture) { m_EditorTexture = isEditorTexture; }
		inline explicit operator bool() const { return m_texID != 0; }

		void Bind();
		void Unbind();
	};

}