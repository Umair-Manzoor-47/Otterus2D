#pragma once
#include <glad/glad.h>
#include "../Essentials/Primitives.h"
#include "BatchRenderer.h"
#include "LineBatchRenderer.h"

namespace otterus_rendering {

	class Renderer {
	public:
		enum class GLCapabilities : GLuint {
			BLEND		= GL_BLEND,
			DEPTH_TEST	= GL_DEPTH_TEST,
			LINE_SMOOTH = GL_LINE_SMOOTH
		};

		enum class BlendingFactors : GLuint {

			ONE					= GL_ONE,
			SRC_ALPHA			= GL_SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA
		};

	private:
		std::vector<Line> m_Lines;
		std::vector<Circle> m_Circles;
		std::vector<Rect> m_Rects;

		std::unique_ptr<LineBatchRenderer> m_LineBatch;
		//std::unique_ptr<CircleBatchRenderer> m_CircleBatch;
		std::unique_ptr<SpriteBatchRenderer> m_SpriteBatch;

	public:
		Renderer();
		~Renderer() = default;

		// OPENGL Render function wrappers
		void SetClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat aplha);
		void ClearBuffers(bool color = true, bool depth = true, bool stencil = false);

		void SetBlendCapabilitiy(BlendingFactors sFactor, BlendingFactors dFactor);
		void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height);

		void SetCapability(GLCapabilities capability, bool enabled);
		bool IsCapabilityEnabled(GLCapabilities capability) const;

		// Primitve Draw calls
		void DrawLine(const Line& line);
		void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const Color& color, float lineWidth = 1.f);

		void DrawRect(const Rect& rect);
		void DrawRect(const glm::vec2& position, float width, float height, const Color& color);

		void DrawFillRect(const Rect& rect);

		void DrawCircle(const Circle& circle);
		void DrawCircle(const glm::vec2& position, float radius, const Color& color, float thickness = 1.f);

		void DrawLines(class Shader& shader, class Camera2D& camera);
		void DrawFilledRects(class Shader& shader, class Camera2D& camera);
		void DrawCircles(class Shader& shader, class Camera2D& camera);

		void ClearPrimitives();



	};


}