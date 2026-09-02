#include "Application.h"
#include <SDL.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Rendering/Essentials/ShaderLoader.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Core/Renderer.h>
#include <Rendering/Essentials/Vertex.h>

#include <entt.hpp>
#include <Core/ECS/Entity.h>
#include <Core/ECS/Components/TransformComponent.h>
#include <Core/ECS/Components/SpriteComponent.h>
#include <Core/ECS/Components/Identification.h>
#include <Core/ECS/Components/PhysicsComponent.h>
#include <Core/ECS/Components/BoxColliderComponent.h>
#include <Core/ECS/Components/CircleColliderComponent.h>


#include <Core/Systems/ScriptingSystem.h>
#include <Core/Systems/RenderSystem.h>
#include <Core/Systems/AnimationSystem.h>
#include <Core/Systems/PhysicsSystem.h>

#include <Core/Resources/AssetManager.h>

#include <Windowing/Inputs/Keyboard.h>
#include <Windowing/Inputs/Mouse.h>

#include <Core/Scripting/InputManager.h>

#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Sounds/SoundPlayer/SoundFxPlayer.h>

#include <Logger/Logger.h>

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
			"OTTERUS2D", 
			640, 480, 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 
			true, 
			SDL_WINDOW_OPENGL |
			SDL_WINDOW_RESIZABLE
		);

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

		auto renderer = std::make_shared<otterus_rendering::Renderer>();

		// Enable Blending
		renderer->SetBlendCapabilitiy(otterus_rendering::Renderer::BlendingFactors::SRC_ALPHA, otterus_rendering::Renderer::BlendingFactors::ONE_MINUS_SRC_ALPHA);


		auto assetManager = std::make_shared<otterus_resources::AssetManager>();
		if (!assetManager) {
			OTTERUS_ERROR("Failed to create Asset Manager.");
			return false;
		}


		// Registry from EnTT 
		m_registry = std::make_unique<otterus_core::ECS::Registry>();
		if (!m_registry) {
			OTTERUS_ERROR("Failed to create the EnTT registry");
			return false;
		}

		// Add Renderer to Registry 
		if (!m_registry->AddToContext<std::shared_ptr<otterus_rendering::Renderer>>(renderer)) {

			OTTERUS_ERROR("Failed to add Renderer to registry context.");
			return false;
		}

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
		
		auto animationSystem = std::make_shared<otterus_core::Systems::AnimationSystem>(*m_registry);
		if (!animationSystem) {

			OTTERUS_ERROR("Failed to create the animation system.");
			return false;
		}
		if (!m_registry->AddToContext<std::shared_ptr<otterus_core::Systems::AnimationSystem>>(animationSystem)) {
			OTTERUS_ERROR("Failed to add Animation System into registry context.");
			return false;
		}

		auto musicPlayer = std::make_shared<otterus_sounds::MusicPlayer>();
		if (!musicPlayer) {
			OTTERUS_ERROR("Failed to create music player.");
			return false;
		}

		if (!m_registry->AddToContext<std::shared_ptr<otterus_sounds::MusicPlayer>>(musicPlayer)) {
		
			OTTERUS_ERROR("Failed to add music player to registry context.");
			return false;
		}
		auto soundFxPlayer = std::make_shared<otterus_sounds::SoundFxPlayer>();
		if (!soundFxPlayer) {
			OTTERUS_ERROR("Failed to create SoundFxPlayer player.");
			return false;
		}

		if (!m_registry->AddToContext<std::shared_ptr<otterus_sounds::SoundFxPlayer>>(soundFxPlayer)) {

			OTTERUS_ERROR("Failed to add Sound FX player to registry context.");
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

		// Create Physics World
		otterus_physics::PhysicsWorld physicsWorld = std::make_shared<b2World>(b2Vec2{0.f, 9.8f});
		if (!m_registry->AddToContext<otterus_physics::PhysicsWorld>(physicsWorld)) {
			OTTERUS_ERROR("Failed to add PhysicsWorld into registry context.");
			return false;
		}

		auto physicsSystem = std::make_shared <otterus_core::Systems::PhysicsSystem>(*m_registry);
		if (!m_registry->AddToContext<std::shared_ptr<otterus_core::Systems::PhysicsSystem>>(physicsSystem)) {
			OTTERUS_ERROR("Failed to add PhysicsSystem into registry context.");
			return false;
		}

		if (!LoadShaders()) {
			OTTERUS_ERROR("Failed to load Shaders.");
			return false;
		}
		otterus_core::Systems::ScriptingSystem::RegisterLuaBindings(*lua, *m_registry);
		otterus_core::Systems::ScriptingSystem::RegisterLuaFunctions(*lua);

		if (!scriptingSystem->LoadMainScript(*lua)) {

			OTTERUS_ERROR("Failed to load the main lua script.");
			return false;
		}
		///
		// Test texture
		assetManager->AddTexture("ball", "./assets/textures/ball.png", true);
		auto ballTexture = assetManager->GetTexture("ball");
		// Test bodies
		using namespace otterus_core::ECS;

		auto& reg = m_registry->GetRegistry();

		auto ent1 = reg.create();

		auto& transform1 = reg.emplace<TransformComponent>(
			ent1,
			TransformComponent{
				.position = glm::vec2{320.0f, 0.0f},
				.scale = glm::vec2{1.f},
				.rotation = 0
			}
		);
		auto& circleCollider = reg.emplace<CircleColliderComponent>(
			ent1,
			CircleColliderComponent{
				.radius = 64.0f
			}
		);

		auto& physicsComp = reg.emplace<PhysicsComponent>(
			ent1,
			PhysicsComponent{
				physicsWorld,
				PhysicsAttributes{
					.type = RigidbodyType::DYNAMIC,
					.density = 100.f,
					.friction = 0.5f,
					.restitution = 0.9f,
					.radius = circleCollider.radius * PIXELS_TO_METERS,
					.gravityScale = 5.f,
					.position = transform1.position,
					.scale = transform1.scale,
					.circle = true,
					.fixedRotation = false
				}
			}
		);

		physicsComp.Init(640, 480);

		auto& sprite = reg.emplace<SpriteComponent>(
			ent1,
			SpriteComponent{
				.width = 32.f,
				.height = 32.f,
				.start_x = 0,
				.start_y = 0,
				.texture_name = "ball"


			}
		);

		sprite.generate_uvs(32.f, 32.f);

		auto ent2 = reg.create();

		auto& transform2 = reg.emplace<TransformComponent>(
			ent2,
			TransformComponent{
				.position = glm::vec2{0.f, 400.0f},
				.scale = glm::vec2{1.f},
				.rotation = 0
			}
		);
		auto& boxCollider = reg.emplace<BoxColliderComponent>(
			ent2,
			BoxColliderComponent{
				.width = 480,
				.height = 48
			}
		);

		auto& physicsComp2 = reg.emplace<PhysicsComponent>(
			ent2,
			PhysicsComponent{
				physicsWorld,
				PhysicsAttributes{
					.type = RigidbodyType::STATIC,
					.density = 1000.f,
					.friction = 0.5f,
					.restitution = 0.2f,
					.gravityScale = 0.f,
					.position = transform2.position,
					.scale = transform2.scale,
					.boxSize = glm::vec2{boxCollider.width, boxCollider.height},
					.boxShape = true,
					.fixedRotation = true
				}
			}
		);

		physicsComp2.Init(640, 480);


		return true;
    }

    bool Application::LoadShaders()
    {
		auto assetManager = m_registry->GetContext<std::shared_ptr<otterus_resources::AssetManager>>();;
		if (!assetManager->AddShader("basic", "assets/shaders/basic_shader.vert", "assets/shaders/basic_shader.frag")) {
			
			OTTERUS_ERROR("Failed to add Basic Shader to AssetManager.");
			return false;
		}

		if (!assetManager->AddShader("color", "assets/shaders/color_shader.vert", "assets/shaders/color_shader.frag")) {

			OTTERUS_ERROR("Failed to add Color Shader to AssetManager.");
			return false;
		}

		if (!assetManager->AddShader("font", "assets/shaders/font_shader.vert", "assets/shaders/font_shader.frag")) {

			OTTERUS_ERROR("Failed to add Font Shader to AssetManager.");
			return false;
		}

		auto& shader = assetManager->GetShader("basic");
		if(shader.GetProgramID() == 0) {
			OTTERUS_ERROR("Failed to Get Basic Shader from AssetManager.");
			return false;
		}
		auto& colorShader = assetManager->GetShader("color");
		if (colorShader.GetProgramID() == 0) {
			OTTERUS_ERROR("Failed to Get Color Shader from AssetManager.");
			return false;
		}


		OTTERUS_LOG("Shader Log {0}", shader.GetProgramID());
		OTTERUS_LOG("Shader Log {0}", colorShader.GetProgramID());

        return true;
    }

    void Application::ProcessEvents()
    {
		auto& inputManager = otterus_core::InputManager::GetInstance();
		auto& keyboard = inputManager.GetKeyboard();
		auto& mouse = inputManager.GetMouse();

		while (SDL_PollEvent(&m_event)) {
			switch (m_event.type) {
			case SDL_QUIT:
				m_isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (m_event.key.keysym.sym == SDLK_ESCAPE) {
					m_isRunning = false;
				}
				keyboard.OnKeyPressed(m_event.key.keysym.sym);
				break;

			case SDL_KEYUP:
				keyboard.OnKeyReleased(m_event.key.keysym.sym);
				break;

			case SDL_MOUSEBUTTONDOWN:
				mouse.OnBtnPressed(m_event.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				mouse.OnBtnReleased(m_event.button.button);
				break;
			case SDL_MOUSEWHEEL:
				mouse.SetMouseWheelX(m_event.wheel.x);
				mouse.SetMouseWheelY(m_event.wheel.y);
				break;
			case SDL_MOUSEMOTION:
				mouse.SetMouseMoving(true);
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

		auto& physicsWorld = m_registry->GetContext<otterus_physics::PhysicsWorld>();
		physicsWorld->Step(
			1.f/60.f,
			10,
			8
		);


		auto& physicsSystem = m_registry->GetContext<std::shared_ptr<otterus_core::Systems::PhysicsSystem>>();
		physicsSystem->Update(m_registry->GetRegistry());

		auto& animationSystem = m_registry->GetContext<std::shared_ptr<otterus_core::Systems::AnimationSystem>>();
		animationSystem->Update();

		auto& inputManager = otterus_core::InputManager::GetInstance();
		auto& keyboard = inputManager.GetKeyboard();
		auto& mouse = inputManager.GetMouse();

		keyboard.Update();
		mouse.Update();

	}

    void Application::Render()
    {	
		auto& renderSystem = m_registry->GetContext<std::shared_ptr<otterus_core::Systems::RenderSystem>>();
		auto& renderer = m_registry->GetContext<std::shared_ptr<otterus_rendering::Renderer>>();
		auto& camera = m_registry->GetContext<std::shared_ptr<otterus_rendering::Camera2D>>();
		auto& assetManager = m_registry->GetContext<std::shared_ptr<otterus_resources::AssetManager>>();

		auto& shader = assetManager->GetShader("color");
		auto& fontShader = assetManager->GetShader("font");


		renderer->SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());

		renderer->SetClearColor(1.f, 1.f, 1.f, 1.f);
		renderer->ClearBuffers(true, false, false);

		auto& scriptSystem = m_registry->GetContext<std::shared_ptr<otterus_core::Systems::ScriptingSystem>>();
		scriptSystem->Render();
		renderSystem->Upate();
		renderer->DrawLines(shader, *camera);
		renderer->DrawAllText(fontShader, *camera);

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

