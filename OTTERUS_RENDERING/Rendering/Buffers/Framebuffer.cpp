#include "Framebuffer.h"
#include "../Essentials/TextureLoader.h"
#include <Logger/Logger.h>

namespace otterus_rendering {
	bool Framebuffer::Initialize()
	{
		glCreateFramebuffers(1, &m_FboID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->GetID(), 0);

		if (m_UseRbo)
		{
			glCreateRenderbuffers(1, &m_RboID);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RboID);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, m_Width, m_Height);
		}

		return false;
	}
	Framebuffer::Framebuffer()
		: Framebuffer(640, 480, false)
	{}
	
	Framebuffer::Framebuffer(int width, int height, bool useRbo)
		: m_FboID{ 0 }, m_RboID{ 0 }, m_Texture{ nullptr },
		m_Width{ width }, m_Height{ height }, m_UseRbo{ useRbo }
	{
		m_Texture = std::move(TextureLoader::Create(Texture::TextureType::FRAMEBUFFER, width, height));

		if (!m_Texture || !Initialize())
		{
			OTTERUS_ASSERT(false && "Failed to create frame buffer.");
			OTTERUS_ERROR("Frame buffer creation failed.");
		}
	}

	Framebuffer::~Framebuffer()
	{
	}
	void Framebuffer::Resize(int width, int height)
	{
	}
	void Framebuffer::CheckResize()
	{
	}
}