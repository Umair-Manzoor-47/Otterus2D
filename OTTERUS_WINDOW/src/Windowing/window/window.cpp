#include "window.h"
#include <iostream>

namespace otterus_windowing {


	Window::Window(const std::string title, int width, int height, int xPos, int yPos, bool vsync, Uint32 flags)
		: m_window(nullptr), m_glContext{}, m_title{ title },
		m_width{ width }, m_height{ height }, m_xPos{ xPos }, m_yPos{ yPos },
		m_windowFlags{flags}
	{
		createNewWindow(flags);

		// ENABLE V-Sync?
		if (vsync) {
		
			if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) std::cout << "Failed to enable v-sync! \n";
		}

		std::cout << "Window created successfully. \n";
	}

	Window::~Window()
	{}

	void Window::SetWindowName(const std::string& title)
	{
		m_title = title;
		SDL_SetWindowTitle(m_window.get(), m_title.c_str());
	}

	void Window::createNewWindow(Uint32 flags)
	{
		m_window = WindowPtr(SDL_CreateWindow(
			m_title.c_str(),
			m_xPos, m_yPos,
			m_width, m_height,
			flags
		));

		// check if window was created
		if (!m_window) {
		
			std::string error = SDL_GetError();
			std::cout << "Failed to create window, " << error << "\n";
		}
	}

}
