#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace otterus_rendering {

	class Camera2D {

	private:
		int m_width, m_height;
		float m_scale;

		glm::vec2 m_position, m_screenOffset;
		glm::mat4 m_cameraMatrix, m_orthoProjection;

		bool m_needsUpdate;

	private:
		void Initialize();
	public:
		Camera2D();
		Camera2D(int width, int height);

		glm::vec2 ScreenCoordToWorld(const glm::vec2& screenCoords) const;
		glm::vec2 WorldCoordToScreen(const glm::vec2& worldCoords) const;

		inline void SetPosition(glm::vec2 newPosition) { m_position = newPosition; m_needsUpdate = true; }
		inline void SetScreenOffset(glm::vec2 screenOffset) { m_screenOffset = screenOffset; m_needsUpdate = true; }
		inline void SetScale(float scale) { m_scale = scale; m_needsUpdate = true; }
		
		inline glm::mat4 GetCameraMatrix() const { return m_cameraMatrix; }
		inline const glm::vec2 GetPosition() const { return m_position; }
		inline const glm::vec2 GetScreenOffset() const { return m_screenOffset; }
		inline const float GetScale() const { return m_scale; }
		inline const int GetWidth() const { return m_width; }
		inline const int GetHeight() const { return m_height; }

		void Update();
		void Reset();
		void Resize( int width, int height );
	};
}