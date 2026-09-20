#pragma once
#include <glad/glad.h>
#include <memory>
#include "../Essentials/Texture.h"

namespace otterus_rendering {

	class Framebuffer
	{
	private:
		GLuint m_FboID, m_RboID;
		std::shared_ptr<Texture> m_Texture;
		int m_Width, m_Height;
		bool m_ShouldResize, m_UseRbo;


		bool Initialize();
		bool CleanUp();

	public:
		Framebuffer();
		Framebuffer(int width, int height, bool useRbo);
		~Framebuffer();

		void Bind();
		void Unbind();

		// Resizing
		void Resize(int width, int height);
		void CheckResize();

		inline const GLuint GetID() const { return m_FboID; };
		inline const GLuint GetTextureID() const { return m_Texture ? m_Texture->GetID() : 0; };
		inline const int GetWidth() const { return m_Width; };
		inline const int GetHeight() const { return m_Height; };

	};

}