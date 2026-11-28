#include "Camera2D.h"


namespace otterus_rendering {
	void Camera2D::Initialize()
	{
		// Init ortho projection
		m_orthoProjection = glm::ortho( 0.f, static_cast<float>( m_width ), static_cast<float>( m_height ), 0.f, 0.f, 1.f );

		m_needsUpdate = true;
		Update();
	}

	Camera2D::Camera2D() :
		Camera2D(640, 480)
	{}

	Camera2D::Camera2D(int width, int height) :
		m_width{ width }, m_height{ height }, m_scale{ 1.f },
		m_position{ glm::vec2{ 0.f } }, m_screenOffset{ 0.f }, m_cameraMatrix{ 1.f }, m_orthoProjection{ 1.f },
		m_needsUpdate{ true }
	{
		Initialize();
	}

	glm::vec2 Camera2D::ScreenCoordToWorld(const glm::vec2& screenCoords) const
	{
		glm::vec2 worldCoords{ screenCoords };
		worldCoords /= m_scale;

		worldCoords -= m_screenOffset;

		worldCoords += m_position;

		return worldCoords;
	}

	glm::vec2 Camera2D::WorldCoordToScreen(const glm::vec2& worldCoords) const
	{
		glm::vec2 screenCoords{ worldCoords };

		screenCoords += m_screenOffset;

		screenCoords *= m_scale;

		screenCoords -= m_position;

		return screenCoords;
	}

	void Camera2D::Update()
	{
		if (!m_needsUpdate) return;
		glm::vec3 scale{ m_scale, m_scale, 1.f };
		glm::vec3 translate{ -m_position.x + m_screenOffset.x, -m_position.y + m_screenOffset.y, 0.f };

		glm::mat4 cameraView = glm::scale(glm::mat4(1.f), scale);
		cameraView = glm::translate(cameraView, translate);
		m_cameraMatrix = m_orthoProjection * cameraView;
		m_needsUpdate = false;
	}

	void Camera2D::Reset()
	{
		m_scale = 1.f;
		m_position = glm::vec2{ 0.f };
		m_screenOffset = glm::vec2{ 0.f };
		m_needsUpdate = true;
	}

	void Camera2D::Resize( int width, int height )
	{
		m_width = width;
		m_height = height;

		Initialize();
	}
}