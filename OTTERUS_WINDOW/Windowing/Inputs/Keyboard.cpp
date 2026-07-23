#include "Keyboard.h"
#include <SDL.h>
#include <Logger.h>

namespace otterus_windowing::Inputs {
    Keyboard::Keyboard() : m_mapButtons{
        {OT_KEY_BACKSPACE, Button{}}, {OT_KEY_TAB, Button{}}, {OT_KEY_CLEAR, Button{}},
        {OT_KEY_RETURN, Button{}}, {OT_KEY_PAUSE, Button{}}, {OT_KEY_ESCAPE, Button{}}, {OT_KEY_SPACE, Button{}},
        {OT_KEY_EXCLAIM, Button{}}, {OT_KEY_QUOTEDBL, Button{}}, {OT_KEY_HASH, Button{}}, {OT_KEY_DOLLAR, Button{}},
        {OT_KEY_AMPERSAND, Button{}}, {OT_KEY_QUOTE, Button{}}, {OT_KEY_LEFTPAREN, Button{}}, {OT_KEY_RIGHTPAREN, Button{}},
        {OT_KEY_ASTERISK, Button{}}, {OT_KEY_PLUS, Button{}}, {OT_KEY_COMMA, Button{}}, {OT_KEY_PERIOD, Button{}},
        {OT_KEY_SLASH, Button{}}, {OT_KEY_0, Button{}}, {OT_KEY_2, Button{}}, {OT_KEY_3, Button{}},
        {OT_KEY_4, Button{}}, {OT_KEY_5, Button{}}, {OT_KEY_6, Button{}}, {OT_KEY_7, Button{}},
        {OT_KEY_8, Button{}}, {OT_KEY_9, Button{}}, {OT_KEY_COLON, Button{}}, {OT_KEY_SEMICOLON, Button{}},
        {OT_KEY_LESS, Button{}}, {OT_KEY_EQUALS, Button{}}, {OT_KEY_GREATER, Button{}}, {OT_KEY_QUESTION, Button{}},
        {OT_KEY_AT, Button{}}, {OT_KEY_LEFTBRACKET, Button{}}, {OT_KEY_BACKSLASH, Button{}}, {OT_KEY_RIGHTBRACKET, Button{}},
        {OT_KEY_CARET, Button{}}, {OT_KEY_UNDERSCORE, Button{}}, {OT_KEY_A, Button{}}, {OT_KEY_B, Button{}},
        {OT_KEY_C, Button{}}, {OT_KEY_D, Button{}}, {OT_KEY_E, Button{}}, {OT_KEY_F, Button{}},
        {OT_KEY_G, Button{}}, {OT_KEY_H, Button{}}, {OT_KEY_I, Button{}}, {OT_KEY_J, Button{}},
        {OT_KEY_K, Button{}}, {OT_KEY_L, Button{}}, {OT_KEY_M, Button{}}, {OT_KEY_N, Button{}},
        {OT_KEY_O, Button{}}, {OT_KEY_P, Button{}}, {OT_KEY_Q, Button{}}, {OT_KEY_R, Button{}},
        {OT_KEY_S, Button{}}, {OT_KEY_T, Button{}}, {OT_KEY_U, Button{}}, {OT_KEY_V, Button{}},
        {OT_KEY_W, Button{}}, {OT_KEY_X, Button{}}, {OT_KEY_Y, Button{}}, {OT_KEY_Z, Button{}},
        {OT_KEY_DELETE, Button{}}, {OT_KEY_CAPSLOCK, Button{}}, {OT_KEY_F1, Button{}}, {OT_KEY_F2, Button{}},
        {OT_KEY_F3, Button{}}, {OT_KEY_F4, Button{}}, {OT_KEY_F5, Button{}}, {OT_KEY_F6, Button{}},
        {OT_KEY_F7, Button{}}, {OT_KEY_F8, Button{}}, {OT_KEY_F9, Button{}}, {OT_KEY_F10, Button{}},
        {OT_KEY_F11, Button{}}, {OT_KEY_F12, Button{}}, {OT_KEY_SCROLLOCK, Button{}}, {OT_KEY_INSERT, Button{}},
        {OT_KEY_HOME, Button{}}, {OT_KEY_PAGEUP, Button{}}, {OT_KEY_PAGEDOWN, Button{}}, {OT_KEY_END, Button{}},
        {OT_KEY_RIGHT, Button{}}, {OT_KEY_LEFT, Button{}}, {OT_KEY_DOWN, Button{}}, {OT_KEY_UP, Button{}},
        {OT_KEY_NUMLOCK, Button{}}, {OT_KEY_KP_DIVIDE, Button{}}, {OT_KEY_KP_MULTIPLY, Button{}}, {OT_KEY_KP_MINUS, Button{}},
        {OT_KEY_KP_PLUS, Button{}}, {OT_KEY_KP_ENTER, Button{}}, {OT_KEY_KP1, Button{}}, {OT_KEY_KP2, Button{}},
        {OT_KEY_KP3, Button{}}, {OT_KEY_KP4, Button{}}, {OT_KEY_KP5, Button{}}, {OT_KEY_KP6, Button{}},
        {OT_KEY_KP7, Button{}}, {OT_KEY_KP8, Button{}}, {OT_KEY_KP9, Button{}}, {OT_KEY_KP0, Button{}},
        {OT_KEY_KP_PERIOD, Button{}}, {OT_KEY_LCTRL, Button{}}, {OT_KEY_LSHIFT, Button{}},
        {OT_KEY_LALT, Button{}}, {OT_KEY_RCTRL, Button{}}, {OT_KEY_RSHIFT, Button{}}, {OT_KEY_RALT, Button{}}
    }
    {}

    void Keyboard::Update()
    {
        for (auto& [key, button] : m_mapButtons) {
            button.Reset();
        }
    }

    void Keyboard::OnKeyPressed(int key)
    {
        if (key == KEY_UNKNOWN) {
            OTTERUS_ERROR("Key [{}] is unknown", key);
            return;
        }
        auto keyItr = m_mapButtons.find(key);
        if (keyItr == m_mapButtons.end()) {
            OTTERUS_ERROR("Key [{}] does not exists", key);
            return;
        }

        keyItr->second.Update(true);
    }

    void Keyboard::OnKeyReleased(int key)
    {
        if (key == KEY_UNKNOWN) {
            OTTERUS_ERROR("Key [{}] is unknown", key);
            return;
        }
        auto keyItr = m_mapButtons.find(key);
        if (keyItr == m_mapButtons.end()) {
            OTTERUS_ERROR("Key [{}] does not exists", key);
            return;
        }

        keyItr->second.Update(false);
    }

    const bool Keyboard::IsKeyPressed(int key) const
    {
        if (key == KEY_UNKNOWN) {
            OTTERUS_ERROR("Key [{}] is unknown", key);
            return false;
        }
        auto keyItr = m_mapButtons.find(key);
        if (keyItr == m_mapButtons.end()) {
            OTTERUS_ERROR("Key [{}] does not exists", key);
            return false;
        }

       return keyItr->second.isPressed;
    }

    const bool Keyboard::IsKeyJustPressed(int key) const
    {
        if (key == KEY_UNKNOWN) {
            OTTERUS_ERROR("Key [{}] is unknown", key);
            return false;
        }
        auto keyItr = m_mapButtons.find(key);
        if (keyItr == m_mapButtons.end()) {
            OTTERUS_ERROR("Key [{}] does not exists", key);
            return false;
        }

        return keyItr->second.justPressed;
    }

    const bool Keyboard::IsKeyJustReleased(int key) const
    {
        if (key == KEY_UNKNOWN) {
            OTTERUS_ERROR("Key [{}] is unknown", key);
            return false;
        }
        auto keyItr = m_mapButtons.find(key);
        if (keyItr == m_mapButtons.end()) {
            OTTERUS_ERROR("Key [{}] does not exists", key);
            return false;
        }

        return keyItr->second.justReleased;
    }
}