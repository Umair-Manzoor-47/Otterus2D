#pragma once
#include <glm/glm.hpp>

namespace otterus_rendering
{
class Camera2D;
}

namespace otterus_core::ECS
{
class Registry;
}

namespace otterus_editor {
	struct Canvas;

	class AbstractTool
	{
	private:
		glm::vec2 m_MouseScreenCoords, m_MouseWorldCoords;
		glm::vec2 m_GUICursorCoords, m_GUIRelativeCoords;
		glm::vec2 m_WindowPos, m_WindowSize;

		bool m_Activated, m_OutOfBounds;

	private:
		void UpdateMouseWorldCoords();
		void CheckOutOfBounds( Canvas& canvas );

	protected:
		enum class EMouseButton
		{
			UNKNOWN = 0,
			LEFT,
			MIDDLE,
			RIGHT,
			LAST
		};

		otterus_core::ECS::Registry* m_Registry{ nullptr };
		otterus_rendering::Camera2D* m_Camera{ nullptr };

	protected:
		bool MouseBtnJustPressed( EMouseButton button );
		bool MouseBtnJustReleased( EMouseButton button );
		bool MouseBtnPressed( EMouseButton button );
		bool MouseMoving();

		// USED: by differemt tools to make adjustments to the mouse position if necessary.
		virtual void ExamineMousePosition() = 0;

		inline void SetMouseWorldCoords( const glm::vec2& newCoords ) { m_MouseWorldCoords = newCoords; }

	public:
		AbstractTool();
		virtual ~AbstractTool() = default;

		// NOTE: Update the tool. If overridden, ensure to call the parent update. This will make sure the
		// mouse world coords are updated accordingly.
		virtual void Update( Canvas& canvas );

		//Sets the current registry to the registry of the current scene and the camera.
		bool SetupTool( otterus_core::ECS::Registry* registry, otterus_rendering::Camera2D* camera );

		inline void SetRelativeCoords( const glm::vec2& relativeCoords ) { m_GUIRelativeCoords = relativeCoords; }
		inline void SetCursorCoords( const glm::vec2& cursorCoords ) { m_GUICursorCoords = cursorCoords; }
		inline void SetWindowPos( const glm::vec2& windowPos ) { m_WindowPos = windowPos; }
		inline void SetWindowSize( const glm::vec2& windowSize ) { m_WindowSize = windowSize; }

		inline const glm::vec2& GetMouseScreenCoords() const { return m_MouseScreenCoords; }
		inline const glm::vec2& GetMouseWorldCoords() const { return m_MouseWorldCoords; }

		inline void Activate() { m_Activated = true; }
		inline void DeActivate() { m_Activated = false; }
		inline const bool IsActivated() const { return m_Activated; }
		inline const bool OutOfBounds() const { return m_OutOfBounds; }
	};
}