#define SDL_MAIN_HANDLED 1;
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "Application.h"

int main() {

	auto& app = otterus_editor::Application::GetInstance();
	app.Run();
	return 0;
}