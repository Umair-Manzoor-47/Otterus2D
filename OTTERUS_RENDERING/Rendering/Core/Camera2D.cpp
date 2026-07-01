#include "Camera2D.h"


namespace otterus_rendering {
	Camera2D::Camera2D() :
		Camera2D(640, 480)
	{}

	otterus_rendering::Camera2D::Camera2D(int width, int height) :
		m_width{ width }, m_height{ height }, m_scale{ 1.f },
		m_position{ glm::vec2{0} }, m_cameraMatrix{ 1.f }, m_orthoProjection{ 1.f },
		m_needsUpdate{ true }
	{
		m_orthoProjection = glm::ortho(
			0.f,								// Left
			static_cast<float>(m_width),		// Right
			static_cast<float>(m_height),		// Top
			0.f,								// Bottom
			-1.f,								// Near
			1.f									// Far
		);
		
		Update();
	}

	void Camera2D::Update()
	{

		if (!m_needsUpdate) return;

		// translate
		glm::vec3 translate{ -m_position.x, -m_position.y, 0.f };
		m_cameraMatrix = glm::translate(m_orthoProjection, translate);

		// scale
		glm::vec3 scale{ m_scale, m_scale, 0.f };
		m_cameraMatrix *= glm::scale(glm::mat4(1.f), scale);

		m_needsUpdate = false;

	}
}