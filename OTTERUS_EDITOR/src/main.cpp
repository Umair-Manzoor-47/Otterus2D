#define SDL_MAIN_HANDLED 1;
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Application.h"

int main() {
#ifndef NDEBUG
	// TODO: SHOW CONSOLE via OS
#ifdef _WIN32
	ShowWindow( GetConsoleWindow(), SW_SHOW );
#else
	// ADD Linux stuff
#endif
#else
#ifdef _WIN32
	ShowWindow( GetConsoleWindow(), SW_HIDE );
#else
	// TODO: HIDE CONSOLE via OS
	// ADD Linux stuff
#endif
#endif
	auto& app = otterus_editor::Application::GetInstance();
	app.Run();
	return 0;
}