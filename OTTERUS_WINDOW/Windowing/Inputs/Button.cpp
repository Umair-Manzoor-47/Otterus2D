#include "Button.h"

void otterus_windowing::Inputs::Button::Update(bool pressed)
{
	justPressed = !isPressed && pressed;
	justReleased = isPressed && !pressed;
	isPressed = pressed;
}

void otterus_windowing::Inputs::Button::Reset()
{
	justPressed = false;
	justReleased = false;
	// NOT Resetting the pressed state as it will be updated
	// through Update()
	
}
