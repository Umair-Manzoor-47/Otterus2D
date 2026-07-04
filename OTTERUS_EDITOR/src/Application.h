#pragma once
#include <Windowing/window/window.h>
#include <glad/glad.h>
#include <Core/ECS/Registry.h>
#include <SDL.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Rendering/Essentials/ShaderLoader.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Essentials/Vertex.h>
#include <entt.hpp>
#include <Core/ECS/Entity.h>
#include <Core/ECS/Components/TransformComponent.h>
#include <Core/ECS/Components/SpriteComponent.h>
#include <Core/ECS/Components/Identification.h>
#include <Core/ECS/Systems/ScriptingSystem.h>


#include <Core/Resources/AssetManager.h>

#include <Logger/Logger.h>

namespace otterus_editor {
	
	class Application
	{
	private:
		std::unique_ptr<otterus_windowing::Window> m_window;
		std::unique_ptr<otterus_core::ECS::Registry> m_registry;

		SDL_Event m_event;
		bool m_isRunning;

		// TODO: Move them to respective classes
		GLuint VAO, VBO, IBO;

	private:
		bool Initialize();
		bool LoadShaders();

		void ProcessEvents();
		void Update();
		void Render();

		void CleanUp();

		Application();

	public:
		static Application& GetInstance();
		~Application();

		void Run();


	};

}
