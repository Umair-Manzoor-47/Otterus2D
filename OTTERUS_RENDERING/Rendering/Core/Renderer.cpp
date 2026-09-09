#include "Renderer.h"
#include "../Essentials/Shader.h"
#include "Camera2D.h"


namespace otterus_rendering {
	Renderer::Renderer()
		: m_LineBatch{ std::make_unique<LineBatchRenderer>() }, 
		m_SpriteBatch{ std::make_unique<SpriteBatchRenderer>() },
		m_TextBatch{ std::make_unique<TextBatchRenderer>() },
		m_CircleBatch{ std::make_unique<CircleBatchRenderer>() },
		m_RectBatch { std::make_unique<RectBatchRenderer>() }
	{}

	void Renderer::SetClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat aplha)
	{
		glClearColor(red, green, blue, aplha);
	}

	void Renderer::ClearBuffers(bool color, bool depth, bool stencil)
	{
		GLbitfield mask = 0;

		if (color)
			mask |= GL_COLOR_BUFFER_BIT;

		if (depth)
			mask |= GL_DEPTH_BUFFER_BIT;

		if (stencil)
			mask |= GL_STENCIL_BUFFER_BIT;

		if (mask != 0)
			glClear(mask);
	}

	void Renderer::SetBlendCapabilitiy(BlendingFactors sFactor, BlendingFactors dFactor)
	{
		glEnable(static_cast<GLenum>(GLCapabilities::BLEND));

		glBlendFunc(
			static_cast<GLenum>(sFactor),
			static_cast<GLenum>(dFactor)
		);

	}

	void Renderer::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		glViewport(
			x,
			y,
			width,
			height
		);
	}

	void Renderer::SetCapability(GLCapabilities capability, bool enabled)
	{
		if (enabled) {
	
			glEnable(static_cast<GLenum>(capability));
		}
		else
		{
			glDisable(static_cast<GLenum>(capability));
		}
	}

	bool Renderer::IsCapabilityEnabled(GLCapabilities capability) const
	{
		return glIsEnabled(static_cast<GLenum>(capability));;
	}

	void Renderer::SetLineWidth(GLfloat lineWidth)
	{
		glLineWidth(lineWidth);
	}

	void Renderer::DrawLine(const Line& line)
	{
		m_Lines.push_back(line);
	}

	void Renderer::DrawLine(const glm::vec2& p1, const glm::vec2& p2, const Color& color, float lineWidth)
	{
		m_Lines.emplace_back(Line{ .p1 = p1, .p2 = p2, .lineWidth = lineWidth, .color = color });
	}

	void Renderer::DrawRect(const Rect& rect)
	{
		// TOP
		DrawLine(Line{
			.p1 = rect.position,
			.p2 = glm::vec2{ rect.position.x + rect.width, rect.position.y},
			.color = rect.color
			});
		
		// BOTTOM
		DrawLine(Line{
				.p1 = glm::vec2{rect.position.x , rect.position.y + rect.height},
				.p2 = glm::vec2{rect.position.x + rect.width, rect.position.y + rect.height},
				.color = rect.color
		});
		// LEFT
		DrawLine(Line{
				.p1 = rect.position,
				.p2 = glm::vec2{rect.position.x, rect.position.y + rect.height},
				.color = rect.color
		});
		// RIGHT
		DrawLine(Line{
				.p1 = glm::vec2{rect.position.x + rect.width, rect.position.y},
				.p2 = glm::vec2{rect.position.x + rect.width, rect.position.y + rect.height},
				.color = rect.color
		});


	}

	void Renderer::DrawRect(const glm::vec2& position, float width, float height, const Color& color)
	{
		DrawRect(Rect{
				.position = position,
				.width = width,
				.height = height,
				.color = color
			});
	}

	void Renderer::DrawFillRect(const Rect& rect)
	{
		m_Rects.push_back(rect);
	}

	void Renderer::DrawCircle(const Circle& circle)
	{

		m_Circles.push_back(circle);
	}

	void Renderer::DrawCircle(const glm::vec2& position, float radius, const Color& color, float thickness)
	{
		m_Circles.push_back(Circle{ .position = position, .lineThickness = thickness, .radius = radius, .color = color });
	}

	void Renderer::DrawText2D(const Text& text)
	{
		m_Text.push_back(text);
	}

	void Renderer::DrawLines(Shader& shader, Camera2D& camera)
	{
		if (m_Lines.empty()) 
			return;

		auto cam_mat = camera.GetCameraMatrix();
		shader.Enable();
		shader.SetUniformMat4("projection", cam_mat);

		m_LineBatch->Begin();

		for (const auto& line : m_Lines)
		{
			m_LineBatch->AddLine(line);
		}
		
		m_LineBatch->End();
		m_LineBatch->Render();
		shader.Disable();
	}


	void Renderer::DrawFilledRects(Shader& shader, Camera2D& camera)
	{
		if (m_Rects.empty()) 
			return;

		auto cam_mat = camera.GetCameraMatrix();
		shader.Enable();
		shader.SetUniformMat4("projection", cam_mat);

		m_RectBatch->Begin();

		for (const auto& rect : m_Rects)
		{
			m_RectBatch->AddRect(rect);
		}

		m_RectBatch->End();
		m_RectBatch->Render();
		shader.Disable();

	}

	void Renderer::DrawCircles(Shader& shader, Camera2D& camera)
	{
		if (m_Circles.empty())
			return;

		auto cam_mat = camera.GetCameraMatrix();
		shader.Enable();
		shader.SetUniformMat4("uProjection", cam_mat);

		m_CircleBatch->Begin();

		for (const auto& circle : m_Circles)
		{
			m_CircleBatch->AddCircle(circle);
		}

		m_CircleBatch->End();
		m_CircleBatch->Render();
		shader.Disable();
	}

	void Renderer::DrawAllText(Shader& shader, Camera2D& camera)
	{
		if (m_Text.empty())
			return;

		auto cam_mat = camera.GetCameraMatrix();
		shader.Enable();
		shader.SetUniformMat4("uProjection", cam_mat);

		m_TextBatch->Begin();

		for (const auto& text : m_Text)
		{
			m_TextBatch->AddText(text.textStr, text.font, text.position, 4, text.wrap,text.color);
		}

		m_TextBatch->End();
		m_TextBatch->Render();
		shader.Disable();
	}

	void Renderer::ClearPrimitives()
	{
		m_Lines.clear();
		m_Rects.clear();
		m_Circles.clear();
		m_Text.clear();
	}
}