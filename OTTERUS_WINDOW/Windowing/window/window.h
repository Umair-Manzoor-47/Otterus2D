#pragma once
#include <OtterusUtilities/SDL_wrappers.h>
#include <string>
#include <SDL.h>

namespace otterus_windowing {
	class Window
	{
	public:
		Window() :
			Window("default_window", 640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true, NULL) {}

		Window(const std::string title, int width, int height, int xPos, int yPos, bool vsync = true,
			Uint32 flags = (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_CAPTURE));
		~Window();

		inline void SetGLContext(SDL_GLContext glContext) { m_glContext = glContext; }
		inline SDL_GLContext& GetGLContext() { return m_glContext; }
		inline WindowPtr& GetWindow() { return m_window; }
		inline const std::string& GetWindowName() const { return m_title; }
		
		inline const int GetXPos() const { return m_xPos; }
		inline const int SetXPos(int xPos) { m_xPos = xPos; }
		inline const int GetYPos() const { return m_xPos; }
		inline const int SetYPos(int yPos) { m_yPos = yPos; }

		inline const int GetWidth() const { return m_width; }
		inline const int GetHeight() const { return m_height; }

		void SetWindowName(const std::string& title);


	private:
		WindowPtr m_window;
		SDL_GLContext m_glContext;
		std::string m_title;
		int m_width, m_height, m_xPos, m_yPos;
		Uint32 m_windowFlags;

		void createNewWindow(Uint32 flags);
	
	};

}