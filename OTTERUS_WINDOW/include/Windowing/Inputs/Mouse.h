#pragma once

#include "Button.h"
#include "MouseButtons.h"
#include <map>

namespace otterus_windowing::Inputs {

	class Mouse
	{
	private:
		std::map<int, Button> m_mapButtons{
		
			{OT_MOUSE_LEFT, Button{}},
			{OT_MOUSE_MIDDLE, Button{}},
			{OT_MOUSE_RIGHT, Button{}},
		};

		int m_X{ 0 }, m_Y{ 0 }, m_WheelX{ 0 }, m_WheelY{ 0 };
		bool m_MouseMoving{false};


	public:
		Mouse() = default;
		~Mouse() =  default;

		void Update();
		void OnBtnPressed(int btn);
		void OnBtnReleased(int btn);

		const bool IsBtnPressed(int btn) const;
		const bool IsBtnJustPressed(int btn) const;
		const bool IsBtnJustReleased(int btn) const;

		const std::tuple<int, int> GetMouseScreenPosition();

		inline void SetMouseWheelX(int wheelX) { m_WheelX = wheelX; }
		inline void SetMouseWheelY(int wheelY) { m_WheelY = wheelY; }
		inline void SetMouseMoving(bool isMoving) { m_MouseMoving = isMoving; }

		inline const int GetMouseWheelX() const { return m_WheelX; }
		inline const int GetMouseWheelY() const { return m_WheelY; }
		inline const bool GetMouseMoving() const { return m_MouseMoving; }


	};

}