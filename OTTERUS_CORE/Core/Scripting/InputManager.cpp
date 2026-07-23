#include "InputManager.h"

namespace otterus_core {

    InputManager::InputManager()
        : m_Keyboard{ std::make_unique<Keyboard>() }
    {
        //m_pKeyboard = std::make_unique<Keyboard>();
    }
	void InputManager::RegisterLuaKeyNames(sol::state& lua)
	{
	  // ==================================================================
      // Register Typewriter Keys
      // ==================================================================
        lua.set("KEY_A", OT_KEY_A);
        lua.set("KEY_B", OT_KEY_B);
        lua.set("KEY_C", OT_KEY_C);
        lua.set("KEY_D", OT_KEY_D);
        lua.set("KEY_E", OT_KEY_E);
        lua.set("KEY_F", OT_KEY_F);
        lua.set("KEY_G", OT_KEY_G);
        lua.set("KEY_H", OT_KEY_H);
        lua.set("KEY_I", OT_KEY_I);
        lua.set("KEY_J", OT_KEY_J);
        lua.set("KEY_K", OT_KEY_K);
        lua.set("KEY_L", OT_KEY_L);
        lua.set("KEY_M", OT_KEY_M);
        lua.set("KEY_N", OT_KEY_N);
        lua.set("KEY_O", OT_KEY_O);
        lua.set("KEY_P", OT_KEY_P);
        lua.set("KEY_Q", OT_KEY_Q);
        lua.set("KEY_R", OT_KEY_R);
        lua.set("KEY_S", OT_KEY_S);
        lua.set("KEY_T", OT_KEY_T);
        lua.set("KEY_U", OT_KEY_U);
        lua.set("KEY_V", OT_KEY_V);
        lua.set("KEY_W", OT_KEY_W);
        lua.set("KEY_X", OT_KEY_X);
        lua.set("KEY_Y", OT_KEY_Y);
        lua.set("KEY_Z", OT_KEY_Z);

        lua.set("KEY_0", OT_KEY_0);
        lua.set("KEY_1", OT_KEY_1);
        lua.set("KEY_2", OT_KEY_2);
        lua.set("KEY_3", OT_KEY_3);
        lua.set("KEY_4", OT_KEY_4);
        lua.set("KEY_5", OT_KEY_5);
        lua.set("KEY_6", OT_KEY_6);
        lua.set("KEY_7", OT_KEY_7);
        lua.set("KEY_8", OT_KEY_8);
        lua.set("KEY_9", OT_KEY_9);

        lua.set("KEY_ENTER", OT_KEY_RETURN);
        lua.set("KEY_BACKSPACE", OT_KEY_BACKSPACE);
        lua.set("KEY_ESC", OT_KEY_ESCAPE);
        lua.set("KEY_SPACE", OT_KEY_SPACE);
        lua.set("KEY_LCTRL", OT_KEY_LCTRL);
        lua.set("KEY_RCTRL", OT_KEY_RCTRL);
        lua.set("KEY_LALT", OT_KEY_LALT);
        lua.set("KEY_RALT", OT_KEY_RALT);
        lua.set("KEY_LSHIFT", OT_KEY_LSHIFT);
        lua.set("KEY_RSHIFT", OT_KEY_RSHIFT);

        // ==================================================================
        //  Register Function Keys
        // ==================================================================
        lua.set("KEY_F1", OT_KEY_F1);
        lua.set("KEY_F2", OT_KEY_F2);
        lua.set("KEY_F3", OT_KEY_F3);
        lua.set("KEY_F4", OT_KEY_F4);
        lua.set("KEY_F5", OT_KEY_F5);
        lua.set("KEY_F6", OT_KEY_F6);
        lua.set("KEY_F7", OT_KEY_F7);
        lua.set("KEY_F8", OT_KEY_F8);
        lua.set("KEY_F9", OT_KEY_F9);
        lua.set("KEY_F10", OT_KEY_F10);
        lua.set("KEY_F11", OT_KEY_F11);
        lua.set("KEY_F12", OT_KEY_F12);

        // ==================================================================
        // Register Cursor Control Keys
        // ==================================================================
        lua.set("KEY_UP", OT_KEY_UP);
        lua.set("KEY_RIGHT", OT_KEY_RIGHT);
        lua.set("KEY_DOWN", OT_KEY_DOWN);
        lua.set("KEY_LEFT", OT_KEY_LEFT);

        // ==================================================================
        // Register Numeric Keypad Keys
        // ==================================================================
        lua.set("KP_KEY_0", OT_KEY_KP0);
        lua.set("KP_KEY_1", OT_KEY_KP1);
        lua.set("KP_KEY_2", OT_KEY_KP2);
        lua.set("KP_KEY_3", OT_KEY_KP3);
        lua.set("KP_KEY_4", OT_KEY_KP4);
        lua.set("KP_KEY_5", OT_KEY_KP5);
        lua.set("KP_KEY_6", OT_KEY_KP6);
        lua.set("KP_KEY_7", OT_KEY_KP7);
        lua.set("KP_KEY_8", OT_KEY_KP8);
        lua.set("KP_KEY_9", OT_KEY_KP9);
        lua.set("KP_KEY_ENTER", OT_KEY_KP_ENTER);

	}

	InputManager& InputManager::GetInstance()
	{
		static InputManager instance{};
		return instance;
	}

	void InputManager::CreateLuaBindings(sol::state& lua)
	{
        RegisterLuaKeyNames(lua);
        auto& inputManager = InputManager::GetInstance();
        auto& keyboard = inputManager.GetKeyboard();
    
        lua.new_usertype<Keyboard>(
            "Keyboard",
            sol::no_constructor,
            "just_pressed", [&](int key) { return keyboard.IsKeyJustPressed(key); },
            "just_released", [&](int key) { return keyboard.IsKeyJustReleased(key); },
            "pressed", [&](int key) { return keyboard.IsKeyPressed(key); }
        );
    }

}
