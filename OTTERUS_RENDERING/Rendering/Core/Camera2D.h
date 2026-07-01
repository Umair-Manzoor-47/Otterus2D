#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace otterus_rendering {

	class Camera2D {

	private:
		int m_width, m_height;
		float m_scale;

		glm::vec2 m_position;
		glm::mat4 m_cameraMatrix, m_orthoProjection;

		bool m_needsUpdate;
	public:
		Camera2D();
		Camera2D(int width, int height);

		inline void SetPosition(glm::vec2 newPosition) { m_position = newPosition; m_needsUpdate = true; }
		inline void SetScale(float scale) { m_scale = scale; m_needsUpdate = true; }
		
		inline glm::mat4 GetCameraMatrix() { return m_cameraMatrix; }
		inline const float GetPosition() const { return m_scale; }


		void Update();

	};
}