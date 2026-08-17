#include "Renderer.h"

namespace otterus_rendering {
	Renderer::Renderer()
		: m_LineBatch{ nullptr }, m_SpriteBatch{nullptr}
	{
	}

	void Renderer::SetClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat aplha)
	{
	}

	void Renderer::ClearBuffers(bool color, bool depth, bool stencil)
	{
	}

	void Renderer::SetBlendCapabilitiy(BlendingFactors sFactor, BlendingFactors dFactor)
	{
	}

	void Renderer::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
	}

	void Renderer::DrawLine(const Line& line)
	{
	}

	void Renderer::DrawLine(const glm::vec2& p1, const glm::vec2& p2, const Color& color, float lineWidth)
	{
	}

	void Renderer::DrawRect(const Rect& rect)
	{
	}

	void Renderer::DrawRect(const glm::vec2& position, float width, float height, const Color& color)
	{
	}

	void Renderer::DrawFillRect(const Rect& rect)
	{
	}

	void Renderer::DrawCircle(const Circle& circle)
	{
	}

	void Renderer::DrawCircle(const glm::vec2& position, float radius, const Color& color, float thickness)
	{
	}

	void Renderer::DrawLines()
	{
	}

	void Renderer::DrawRects()
	{
	}

	void Renderer::DrawFilledRects()
	{
	}

	void Renderer::DrawCircles()
	{
	}

	void Renderer::ClearPrimitives()
	{
	}
}