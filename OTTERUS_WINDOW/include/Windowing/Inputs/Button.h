#pragma once

namespace otterus_windowing::Inputs {

	struct Button
	{
		bool isPressed{ false }, justPressed{ false }, justReleased{ false };
		void Update(bool pressed);
		void Reset();
	};


}