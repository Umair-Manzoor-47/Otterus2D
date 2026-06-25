#define SDL_MAIN_HANDLED 1;
#include <windowing/window/window.h>
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>

int main() {

	bool running { true };

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
	
		std::string error = SDL_GetError();
		std::cout << "Failed to init SDL, " << error << "\n";

		running = false;
		return -1;
	}

	// Setup Open GL
	if (SDL_GL_LoadLibrary(NULL) != 0) {

		std::string error = SDL_GetError();
		std::cout << "Failed to get OpenGL lib, " << error << "\n";

		running = false;
		return -1;
	}

	// Set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Create Window
	otterus_windowing::Window window("Test Window", 640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true, SDL_WINDOW_OPENGL);

	if (!window.GetWindow()) {
		std::cout << "Failed to create widow.\n"<< std::endl;
		return -1;
	}


	// Create OPENGL CONTEXT
	window.SetGLContext(SDL_GL_CreateContext(window.GetWindow().get()));
	if (!window.GetGLContext()) {

		std::string error = SDL_GetError();
		std::cout << "Failed to create openGL context, " << error << "\n";

		running = false;
		return -1;
	}

	SDL_GL_MakeCurrent(window.GetWindow().get(), window.GetGLContext());
	SDL_GL_SetSwapInterval(1);

	if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
	{
		std::cout << "Failed to load openGL --> GLAD" << std::endl;
	
		running = false;

		return -1;
	}
	SDL_Event event{};

	while (running) {
	
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					running = false;
				}
				break;
			
		    default:
				break;

			}
		}

		glViewport(
			window.GetXPos(),
			window.GetYPos(),
			window.GetWidth(),
			window.GetHeight()
		);

		glClearColor(0.3f, 0.2f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

	
		SDL_GL_SwapWindow(window.GetWindow().get());
	}

	std::cout << "EXIT" << std::endl;

	return 0;
}