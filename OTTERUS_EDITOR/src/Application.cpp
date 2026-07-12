#include "Application.h"


namespace otterus_editor {

    bool Application::Initialize()
    {
		OTTERUS_INIT_LOGS(true, true);


		// Initialize SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

			std::string error = SDL_GetError();
			std::cout << "Failed to init SDL, " << error << "\n";

			return false;
		}

		// Setup Open GL
		if (SDL_GL_LoadLibrary(NULL) != 0) {

			std::string error = SDL_GetError();
			std::cout << "Failed to get OpenGL lib, " << error << "\n";

			return false;
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
		m_window = std::make_unique<otterus_windowing::Window>(
			"Test Window", 
			640, 480, 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 
			true, 
			SDL_WINDOW_OPENGL);

		if (!m_window->GetWindow()) {
			std::cout << "Failed to create widow.\n" << std::endl;
			return false;
		}


		// Create OPENGL CONTEXT
		m_window->SetGLContext(SDL_GL_CreateContext(m_window->GetWindow().get()));
		if (!m_window->GetGLContext()) {

			std::string error = SDL_GetError();
			std::cout << "Failed to create openGL context, " << error << "\n";

			return false;
		}

		SDL_GL_MakeCurrent(m_window->GetWindow().get(), m_window->GetGLContext());
		SDL_GL_SetSwapInterval(1);


		// initialize Glad
		if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
		{
			std::cout << "Failed to load openGL --> GLAD" << std::endl;
			return false;
		}

		// Enable Blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



		auto assetManager = std::make_shared<otterus_resources::AssetManager>();
		if (!assetManager) {
			OTTERUS_ERROR("Failed to create Asset Manager.");
			return false;
		}

		if (!assetManager->AddTexture("Tile", "assets/textures/tiles.png", true)) {
			OTTERUS_ERROR("Failed to create and add Texture.");
			return false;
		}


		// Registry from EnTT 
		m_registry = std::make_unique<otterus_core::ECS::Registry>();
		if (!m_registry) {
			OTTERUS_ERROR("Failed to create the EnTT registry");
			return false;
		}


		GLuint indices[] = {
			0, 1, 3,  // first triangle
			1, 2, 3   // second triangle
		};

		// Create script module
		auto lua = std::make_shared<sol::state>();

		if (!lua) {
		
			OTTERUS_ERROR("Failed to create the lua state.");
			return false;
		}

		lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::table, sol::lib::io, sol::lib::string);

		if (!m_registry->AddToContext<std::shared_ptr<sol::state>>(lua)) {
		
			OTTERUS_ERROR("Failed to add sol::state to registry context.");
			return false;
		}

		auto scriptingSystem = std::make_shared<otterus_core::Systems::ScriptingSystem>(*m_registry);
		if (!scriptingSystem) {

			OTTERUS_ERROR("Failed to create the scripting system.");
			return false;
		}

		

		if (!m_registry->AddToContext<std::shared_ptr<otterus_core::Systems::ScriptingSystem>>(scriptingSystem)) {

			OTTERUS_ERROR("Failed to add otterus_core::Systems::ScriptingSystem to registry context.");
			return false;
		}

		auto renderSystem = std::make_shared<otterus_core::Systems::RenderSystem>(*m_registry);
		if (!renderSystem) {

			OTTERUS_ERROR("Failed to create the render system.");
			return false;
		}
		if (!m_registry->AddToContext<std::shared_ptr<otterus_core::Systems::RenderSystem>>(renderSystem)) {
			OTTERUS_ERROR("Failed to add Render System into registry context.");
			return false;
		}

		// Create temp camera
		auto camera = std::make_shared<otterus_rendering::Camera2D>();

		if (!m_registry->AddToContext<std::shared_ptr<otterus_resources::AssetManager>>(assetManager)) {
			OTTERUS_ERROR("Failed to add AssetManager into registry context.");
			return false;
		}

		if (!m_registry->AddToContext<std::shared_ptr<otterus_rendering::Camera2D>>(camera)) {
			OTTERUS_ERROR("Failed to add Camera2D into registry context.");
			return false;
		}

		if (!LoadShaders()) {
			OTTERUS_ERROR("Failed to load Shaders.");
			return false;
		}
		otterus_core::Systems::ScriptingSystem::RegisterLuaBindings(*lua, *m_registry);

		if (!scriptingSystem->LoadMainScript(*lua)) {

			OTTERUS_ERROR("Failed to load the main lua script.");
			return false;
		}

    }

    bool Application::LoadShaders()
    {
		auto assetManager = m_registry->GetContext<std::shared_ptr<otterus_resources::AssetManager>>();;
		if (!assetManager->AddShader("basic", "assets/shaders/basic_shader.vert", "assets/shaders/basic_shader.frag")) {
			
			OTTERUS_ERROR("Failed to add Basic Shader to AssetManager.");
			return false;
		}


		auto& shader = assetManager->GetShader("basic");
		if(shader.GetProgramID() == 0) {
			OTTERUS_ERROR("Failed to Get Basic Shader from AssetManager.");
			return false;
		}

		OTTERUS_LOG("Shader Log {0}", shader.GetProgramID());
        return true;
    }

    void Application::ProcessEvents()
    {
		while (SDL_PollEvent(&m_event)) {
			switch (m_event.type) {
			case SDL_QUIT:
				m_isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (m_event.key.keysym.sym == SDLK_ESCAPE) {
					m_isRunning = false;
				}
				break;

			default:
				break;

			}
		}
	}

    void Application::Update()
    {
		auto& camera = m_registry->GetContext<std::shared_ptr<otterus_rendering::Camera2D>>();
		if (!camera) {
		
			OTTERUS_ERROR("Failed to get Camera2D from Registry.");
			return;
		}

		camera->Update();

		auto& scriptSystem = m_registry->GetContext<std::shared_ptr<otterus_core::Systems::ScriptingSystem>>();
		scriptSystem->Update();
	
	}

    void Application::Render()
    {	
		auto& renderSystem = m_registry->GetContext<std::shared_ptr<otterus_core::Systems::RenderSystem>>();

		glViewport(
			0,
			0,
			m_window->GetWidth(),
			m_window->GetHeight()
		);

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		auto& scriptSystem = m_registry->GetContext<std::shared_ptr<otterus_core::Systems::ScriptingSystem>>();
		scriptSystem->Render();
		renderSystem->Upate();

		SDL_GL_SwapWindow(m_window->GetWindow().get());

	}

    void Application::CleanUp()
    {
		SDL_Quit();
	}

    Application::Application():
        m_window{ nullptr }, m_registry{nullptr}, m_event {}, m_isRunning{ true }
    {}

    Application& Application::GetInstance()
    {
		static Application app{};
		return app;
    }

    Application::~Application()
    {}

    void Application::Run()
    {
		if (!Initialize()) {
			OTTERUS_ERROR("Failed to Initialize Editor.");
			return;
		}
		while (m_isRunning) {
			ProcessEvents();
			Update();
			Render();
		}

		CleanUp();
	
	}

}

