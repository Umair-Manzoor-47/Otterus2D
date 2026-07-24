#include "Mouse.h"
#include <SDL.h>
#include <Logger.h>


namespace otterus_windowing::Inputs {
    void Mouse::Update()
    {
        for (auto& [btn, button] : m_mapButtons)
            button.Reset();

        m_WheelX = 0;
        m_WheelY = 0;
        m_MouseMoving = false;
    }

    void Mouse::OnBtnPressed(int btn)
    {
        if (btn == OT_MOUSE_UNKNOWN) {
            OTTERUS_ERROR("Mouse button [{}] is UNKNOWN.", btn);
            return;
        }

        auto btnItr = m_mapButtons.find(btn);
        if (btnItr == m_mapButtons.end()) {
            OTTERUS_ERROR("Mouse button [{}] doesn't exist.", btn);
            return;
        }

        btnItr->second.Update(true);
    }

    void Mouse::OnBtnReleased(int btn)
    {
        if (btn == OT_MOUSE_UNKNOWN) {
            OTTERUS_ERROR("Mouse button [{}] is UNKNOWN.", btn);
            return;
        }

        auto btnItr = m_mapButtons.find(btn);
        if (btnItr == m_mapButtons.end()) {
            OTTERUS_ERROR("Mouse button [{}] doesn't exist.", btn);
            return;
        }

        btnItr->second.Update(false);
    }

    const bool Inputs::Mouse::IsBtnPressed(int btn) const
    {
        if (btn == OT_MOUSE_UNKNOWN) {
            OTTERUS_ERROR("Mouse button [{}] is UNKNOWN.", btn);
            return false;
        }

        auto btnItr = m_mapButtons.find(btn);
        if (btnItr == m_mapButtons.end()) {
            OTTERUS_ERROR("Mouse button [{}] doesn't exist.", btn);
            return false;
        }

        return btnItr->second.isPressed;
        
    }

    const bool Mouse::IsBtnJustPressed(int btn) const
    {
        if (btn == OT_MOUSE_UNKNOWN) {
            OTTERUS_ERROR("Mouse button [{}] is UNKNOWN.", btn);
            return false;
        }

        auto btnItr = m_mapButtons.find(btn);
        if (btnItr == m_mapButtons.end()) {
            OTTERUS_ERROR("Mouse button [{}] doesn't exist.", btn);
            return false;
        }

        return btnItr->second.justPressed;
    }

    const bool Mouse::IsBtnJustReleased(int btn) const
    {
        if (btn == OT_MOUSE_UNKNOWN) {
            OTTERUS_ERROR("Mouse button [{}] is UNKNOWN.", btn);
            return false;
        }

        auto btnItr = m_mapButtons.find(btn);
        if (btnItr == m_mapButtons.end()) {
            OTTERUS_ERROR("Mouse button [{}] doesn't exist.", btn);
            return false;
        }

        return btnItr->second.justReleased;
    }

    const std::tuple<int, int> Mouse::GetMouseScreenPosition()
    {
        SDL_GetMouseState(&m_X, &m_Y);
        return std::make_tuple(m_X, m_Y);
    }

}
