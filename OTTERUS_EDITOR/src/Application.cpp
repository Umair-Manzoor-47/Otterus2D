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
#include <Rendering/Buffers/Framebuffer.h>

#include <Core/ECS/Entity.h>
#include <Core/ECS/Components/PhysicsComponent.h>


#include <Core/Systems/ScriptingSystem.h>
#include <Core/Systems/LightingSystem.h>
#include <Core/Systems/RenderSystem.h>
#include <Core/Systems/AnimationSystem.h>
#include <Core/Systems/PhysicsSystem.h>
#include <core/Systems/RenderShapeSystem.h>
#include <core/Systems/RenderUISystem.h>

#include <Core/Resources/AssetManager.h>

#include <Windowing/Inputs/Keyboard.h>
#include <Windowing/Inputs/Mouse.h>

#include <Core/Scripting/InputManager.h>

#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Sounds/SoundPlayer/SoundFxPlayer.h>

#include <Logger/Logger.h>

// TODO: Remove to ImGUI class
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <SDL_opengl.h>
// ===========================
#include "editor/displays/SceneDisplay.h"
#include "editor/displays/LogDisplay.h"
#include "editor/displays/TilesetDisplay.h"
#include "editor/displays/TilemapDisplay.h"
#include "editor/displays/AssetDisplay.h"
#include "editor/displays/MenuDisplay.h"
#include "editor/displays/TileDetailsDisplay.h"
#include "editor/displays/SceneHierarchyDisplay.h"

#include <Physics/ContactListener.h>
#include <Core/ECS/MainRegistry.h>

#include <Core/CoreUtilities/CoreEngineData.h>
#include "editor/utilities/DrawComponentUtils.h"

#include "editor/utilities/editor_textures.h"
#include "editor/utilities/EditorFrameBuffer.h"
#include "editor/utilities/EditorTheme.h"
#include "editor/systems/GridSystem.h"
#include "editor/scene/SceneManager.h"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

namespace otterus_editor {

    bool Application::Initialize()
    {
		OTTERUS_INIT_LOGS(false, true);


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

		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);

		// Create Window
		m_window = std::make_unique<otterus_windowing::Window>(
			"OTTERUS2D", 
			displayMode.w, displayMode.h, 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 
			true, 
			SDL_WINDOW_OPENGL		 |
			SDL_WINDOW_RESIZABLE	 |
			SDL_WINDOW_MAXIMIZED	 |
			SDL_WINDOW_MOUSE_CAPTURE
		);

		if (!m_window->GetWindow()) {
			std::cout << "Failed to create widow.\n" << std::endl;
			return false;
		}

		SetWindowIcon();


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

		// GPU logging
		OTTERUS_LOG("OpenGL Vendor:   {0}", (const char*)glGetString(GL_VENDOR));
		OTTERUS_LOG("OpenGL Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		OTTERUS_LOG("OpenGL Version:  {0}", (const char*)glGetString(GL_VERSION));

    	auto& mainRegistry = MAIN_REGISTRY();

		mainRegistry.Initialize();


		if (!InitImGui()) {
			OTTERUS_ERROR("Failed to initialize ImGUI.");
			return false;
		}

		if (!LoadShaders()) {
			OTTERUS_ERROR("Failed to load Shaders.");
			return false;
		}
    	if (!LoadEditorTextures())
    	{
    		OTTERUS_ERROR("Failed to load EditorTextures.");
    		return false;
    	}

    	if (!mainRegistry.GetAssetManager().CreateDefaultFonts())
	    {
			OTTERUS_ERROR("Failed to load default fonts.");
			return false;
	    }


		auto editorFramebuffers = std::make_shared<EditorFramebuffers>();

		if (!editorFramebuffers) {
			OTTERUS_ERROR("Failed to create Editor Framebuffer.");
			return false;
		}
		if (!mainRegistry.AddToContext<std::shared_ptr<EditorFramebuffers>>(editorFramebuffers)) {
			OTTERUS_ERROR("Failed to add Editor Framebuffer into registry context.");
			return false;
		}

    	editorFramebuffers->mapFramebuffers.emplace( FramebufferType::SCENE,
											  std::make_shared<otterus_rendering::Framebuffer>( 640, 480, false ) );

    	editorFramebuffers->mapFramebuffers.emplace( FramebufferType::TILEMAP,
													  std::make_shared<otterus_rendering::Framebuffer>( 640, 480, false ) );

    	if ( !mainRegistry.AddToContext<std::shared_ptr<GridSystem>>( std::make_shared<GridSystem>() ) )
    	{
    		OTTERUS_ERROR( "Failed add the grid system registry context!" );
    		return false;
    	}

		if (!CreateDisplays()) {
			OTTERUS_ERROR("Failed to create displays.");

			return false;
		}

    	DrawComponentUtils::RegisterUIComponent<otterus_core::ECS::Identification>();
    	DrawComponentUtils::RegisterUIComponent<otterus_core::ECS::TransformComponent>();
    	DrawComponentUtils::RegisterUIComponent<otterus_core::ECS::SpriteComponent>();
    	DrawComponentUtils::RegisterUIComponent<otterus_core::ECS::AnimationComponent>();
    	DrawComponentUtils::RegisterUIComponent<otterus_core::ECS::PhysicsComponent>();
    	DrawComponentUtils::RegisterUIComponent<otterus_core::ECS::TextComponent>();
    	DrawComponentUtils::RegisterUIComponent<otterus_core::ECS::RigidBodyComponent>();
    	DrawComponentUtils::RegisterUIComponent<otterus_core::ECS::BoxColliderComponent>();
    	DrawComponentUtils::RegisterUIComponent<otterus_core::ECS::CircleColliderComponent>();
    	DrawComponentUtils::RegisterUIComponent<otterus_core::ECS::TileComponent>();
    	DrawComponentUtils::RegisterUIComponent<otterus_core::ECS::LightComponent>();

    	otterus_core::CoreEngineData::RegisterMetaFunctions();

    	//TODO: REMOVE
    	SCENE_MANAGER().AddScene("DefaultScene");
    	SCENE_MANAGER().AddScene("Splash");
    	SCENE_MANAGER().AddScene("MainMenu");

		return true;
    }

    bool Application::LoadShaders()
    {

		auto assetManager = MAIN_REGISTRY().GetContext<std::shared_ptr<otterus_resources::AssetManager>>();
		if (!assetManager->AddShader("basic", "assets/shaders/basic_shader.vert", "assets/shaders/basic_shader.frag")) {
			
			OTTERUS_ERROR("Failed to add Basic Shader to AssetManager.");
			return false;
		}

		if (!assetManager->AddShader("color", "assets/shaders/color_shader.vert", "assets/shaders/color_shader.frag")) {

			OTTERUS_ERROR("Failed to add Color Shader to AssetManager.");
			return false;
		}
		if (!assetManager->AddShader("circle", "assets/shaders/circle_shader.vert", "assets/shaders/circle_shader.frag")) {

			OTTERUS_ERROR("Failed to add Circle Shader to AssetManager.");
			return false;
		}

		if (!assetManager->AddShader("font", "assets/shaders/font_shader.vert", "assets/shaders/font_shader.frag")) {

			OTTERUS_ERROR("Failed to add Font Shader to AssetManager.");
			return false;
		}

		if (!assetManager->AddShader("light", "assets/shaders/light_shader.vert", "assets/shaders/light_shader.frag")) {

			OTTERUS_ERROR("Failed to add Light Shader to AssetManager.");
			return false;
		}

		if (!assetManager->AddShader("composite", "assets/shaders/composite_shader.vert", "assets/shaders/composite_shader.frag")) {

			OTTERUS_ERROR("Failed to add Composite Shader to AssetManager.");
			return false;
		}

		auto shader = assetManager->GetShader("basic");
		if(!shader || shader->GetProgramID() == 0) {
			OTTERUS_ERROR("Failed to Get Basic Shader from AssetManager.");
			return false;
		}
		auto colorShader = assetManager->GetShader("color");
		if (!colorShader || colorShader->GetProgramID() == 0) {
			OTTERUS_ERROR("Failed to Get Color Shader from AssetManager.");
			return false;
		}

		auto circleShader = assetManager->GetShader("circle");
		if (!circleShader || circleShader->GetProgramID() == 0) {
			OTTERUS_ERROR("Failed to Get Circle Shader from AssetManager.");
			return false;
		}

		auto lightShader = assetManager->GetShader("light");
		if (!lightShader || lightShader->GetProgramID() == 0) {
			OTTERUS_ERROR("Failed to Get Light Shader from AssetManager.");
			return false;
		}

		auto compositeShader = assetManager->GetShader("composite");
		if (!compositeShader || compositeShader->GetProgramID() == 0) {
			OTTERUS_ERROR("Failed to Get Composite Shader from AssetManager.");
			return false;
		}

		OTTERUS_LOG("Shader Log {0}", shader->GetProgramID());
		OTTERUS_LOG("Shader Log {0}", colorShader->GetProgramID());

        return true;
    }

    void Application::SetWindowIcon()
    {
        int width{ 0 }, height{ 0 }, channels{ 0 };
        unsigned char* pixels = SOIL_load_image_from_memory(
            otterus_app_icon,
            static_cast<int>(otterus_app_icon_size),
            &width,
            &height,
            &channels,
            SOIL_LOAD_RGBA
        );
        if (!pixels)
            return;

        if (SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(
                pixels, width, height, 32, width * 4, SDL_PIXELFORMAT_RGBA32))
        {
            SDL_SetWindowIcon(m_window->GetWindow().get(), surface);
            SDL_FreeSurface(surface);
        }
        SOIL_free_image_data(pixels);
    }

    bool Application::LoadEditorTextures()
    {
        struct EmbeddedTexture
        {
            const char* name;
            const unsigned char* data;
            size_t size;
        };

        const EmbeddedTexture textures[] = {
            { "play_button", play_button, sizeof(play_button) },
            { "stop_button", stop_button, sizeof(stop_button) },
            { "music_icon",  music_icon,  sizeof(music_icon) },
            { "scene_icon",  scene_icon,  sizeof(scene_icon) },
        };

        auto& assetManager = MAIN_REGISTRY().GetAssetManager();
        for (const auto& tex : textures)
        {
            if (!assetManager.AddTextureFromMemory(tex.name, tex.data, tex.size))
            {
                OTTERUS_ERROR("Failed to load texture [{}] from memory.", tex.name);
                return false;
            }
            assetManager.GetTexture(tex.name)->SetIsEditorTexture(true);
        }

        return true;
    }

    void Application::ProcessEvents()
    {
		auto& inputManager = otterus_core::InputManager::GetInstance();
		auto& keyboard = inputManager.GetKeyboard();
		auto& mouse = inputManager.GetMouse();

		while (SDL_PollEvent(&m_event)) {
			ImGui_ImplSDL2_ProcessEvent(&m_event);
			switch (m_event.type) {
			case SDL_QUIT:
				m_isRunning = false;
				break;
			case SDL_KEYDOWN:
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
				break;
			case SDL_WINDOWEVENT:
				switch (m_event.window.event)
				{
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					m_window->SetWidth(m_event.window.data1);
					m_window->SetHeight(m_event.window.data2);
					break;
				default:
					break;
				}
				break;
			default:
				break;

			}
		}
	}

    void Application::Update()
    {
		auto& coreGlobals = CORE_GLOBALS();

		coreGlobals.UpdateDeltaTime();

    	auto& mainRegistry = MAIN_REGISTRY();
    	auto& displayHolder = mainRegistry.GetContext<std::shared_ptr<DisplayHolder>>();

    	for ( const auto& display : displayHolder->displays )
    		display->Update();

		auto& inputManager = otterus_core::InputManager::GetInstance();
		auto& keyboard = inputManager.GetKeyboard();
		auto& mouse = inputManager.GetMouse();

		keyboard.Update();
		mouse.Update();

	}

    void Application::Render()
    {	

		Begin();
		RenderImGui();
		End();

		SDL_GL_SwapWindow(m_window->GetWindow().get());
	}

    void Application::CleanUp()
    {
		SDL_Quit();
	}

	bool Application::CreateDisplays()
	{
		auto& mainRegistry = MAIN_REGISTRY();

		auto displayHolder = std::make_shared<DisplayHolder>();

		if (!mainRegistry.AddToContext<std::shared_ptr<DisplayHolder>>(displayHolder))
		{
			OTTERUS_ERROR("Failed to add displayHolder into main registry context.");
			return false;
		}

		auto sceneDisplay = std::make_unique<SceneDisplay>();

		if (!sceneDisplay) {
			OTTERUS_ERROR("Failed to create SceneDisplay.");
			return false;
		}

		auto logDisplay = std::make_unique<LogDisplay>();

		if (!logDisplay) {
			OTTERUS_ERROR("Failed to create LogDisplay.");
			return false;
		}
    	auto tilesetDisplay = std::make_unique<TilesetDisplay>();

    	if (!tilesetDisplay) {
    		OTTERUS_ERROR("Failed to create TilesetDisplay.");
    		return false;
    	}

    	auto tilemapDisplay = std::make_unique<TilemapDisplay>();
    	if ( !tilemapDisplay )
    	{
    		OTTERUS_ERROR( "Failed to Create TilemapDisplay!" );
    		return false;
    	}
    	auto assetDisplay = std::make_unique<AssetDisplay>();
    	if ( !assetDisplay )
    	{
    		OTTERUS_ERROR( "Failed to Create AssetDisplay!" );
    		return false;
    	}
    	auto menuDisplay = std::make_unique<MenuDisplay>();
    	if ( !menuDisplay )
    	{
    		OTTERUS_ERROR( "Failed to Create Menu Display!" );
    		return false;
    	}
    	auto tileDetailsDisplay = std::make_unique<TileDetailsDisplay>();
    	if ( !tileDetailsDisplay )
    	{
    		OTTERUS_ERROR( "Failed to Create TileDetailsDisplay!" );
    		return false;
    	}
    	auto sceneHierarchyDisplay = std::make_unique<SceneHierarchyDisplay>();
    	if ( !sceneHierarchyDisplay )
    	{
    		OTTERUS_ERROR( "Failed to Create Scene Hierarchy Display!" );
    		return false;
    	}
		// TODO: Other displays as needed

		displayHolder->displays.push_back(std::move(sceneDisplay));
		displayHolder->displays.push_back(std::move(logDisplay));
		displayHolder->displays.push_back(std::move(tilesetDisplay));
    	displayHolder->displays.push_back(std::move(tilemapDisplay));
    	displayHolder->displays.push_back(std::move(assetDisplay));
    	displayHolder->displays.push_back( std::move(menuDisplay));
    	displayHolder->displays.push_back( std::move(tileDetailsDisplay));
    	displayHolder->displays.push_back( std::move(sceneHierarchyDisplay));

		return true;
	}

	bool Application::InitImGui()
	{
		const char* glslVersion = "#version 450";
		IMGUI_CHECKVERSION();

		if (!ImGui::CreateContext())
		{
			OTTERUS_ERROR("Failed to create ImGui context.");
			return false;
		}

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.ConfigWindowsMoveFromTitleBarOnly = true;

		EditorTheme::ConfigureFonts(io);
		EditorTheme::Apply();

		if (!ImGui_ImplSDL2_InitForOpenGL(
			m_window->GetWindow().get(),
			m_window->GetGLContext()
		))
		{
			OTTERUS_ERROR("Failed to initialize ImGUI SDL2 impl for OpenGL (ImGui_ImplSDL2_InitForOpenGL).");
			return false;
		}
		if (!ImGui_ImplOpenGL3_Init(glslVersion))
		{
			OTTERUS_ERROR("Failed to initialize ImGUI OpenGL3 impl (ImGui_ImplOpenGL3_Init).");
			return false;
		}

		return true;
	}

	void Application::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void Application::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

			SDL_GLContext backupContext = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			SDL_GL_MakeCurrent(
				m_window->GetWindow().get(),
				backupContext
			);
		}
	}

	void Application::RenderImGui()
	{
		const auto dockSpaceId = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);
		if (static auto firstTime = true; firstTime) [[unlikely]]
		{
			firstTime = false;

			ImGui::DockBuilderRemoveNode(dockSpaceId);
			ImGui::DockBuilderAddNode(dockSpaceId);

			auto centerNodeId = dockSpaceId;
			const auto leftNodeId =
				ImGui::DockBuilderSplitNode(centerNodeId, ImGuiDir_Left, 0.2f, nullptr, &centerNodeId);
			const auto rightNodeId =
				ImGui::DockBuilderSplitNode(centerNodeId, ImGuiDir_Right, 0.2f, nullptr, &centerNodeId);

			const auto bottomNodeId = ImGui::DockBuilderSplitNode(centerNodeId, ImGuiDir_Down, 0.25f, nullptr, &centerNodeId);
			ImGui::DockBuilderDockWindow("Scene", centerNodeId);
			ImGui::DockBuilderDockWindow("Scene Hierarchy", leftNodeId);
			ImGui::DockBuilderDockWindow("Tilemap Editor", centerNodeId);
			ImGui::DockBuilderDockWindow("Assets", bottomNodeId);
			ImGui::DockBuilderDockWindow("Console", bottomNodeId);
			ImGui::DockBuilderDockWindow("Tileset", bottomNodeId);
			ImGui::DockBuilderDockWindow("Tile Details", rightNodeId);
			ImGui::DockBuilderDockWindow("Inspector", rightNodeId);

			ImGui::DockBuilderFinish(dockSpaceId);
		}

		auto& mainRegistry = MAIN_REGISTRY();
		auto& displayHolder = mainRegistry.GetContext<std::shared_ptr<DisplayHolder>>();

		for (const auto& display : displayHolder->displays)
		{
			display->Draw();
		}

	}

    Application::Application():
        m_window{ nullptr }, m_event {}, m_isRunning{ true }
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

