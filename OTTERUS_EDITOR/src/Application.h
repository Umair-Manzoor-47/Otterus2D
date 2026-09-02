#pragma once
#include <Windowing/window/window.h>
#include <glad/glad.h>
#include <Core/ECS/Registry.h>

namespace otterus_editor {
	
	class Application
	{
	private:
		std::unique_ptr<otterus_windowing::Window> m_window;
		std::unique_ptr<otterus_core::ECS::Registry> m_registry;

		SDL_Event m_event;
		bool m_isRunning;

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
