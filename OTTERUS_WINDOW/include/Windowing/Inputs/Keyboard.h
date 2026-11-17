#pragma once
#include "Button.h"
#include "Keys.h"
#include <map>

namespace otterus_windowing::Inputs{
	class Keyboard {
	private:
		std::map<int, Button> m_mapButtons;

	public:
		Keyboard();
		~Keyboard() = default;

		void Update();
		void OnKeyPressed(int key);
		void OnKeyReleased(int key);

		const bool IsKeyPressed(int key) const;
		const bool IsKeyJustPressed(int key) const;
		const bool IsKeyJustReleased(int key) const;

	};

}
