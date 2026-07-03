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

		// Create temp texture
		auto texture = assetManager->GetTexture("Tile");

		int width = texture.GetWidth();
		int height = texture.GetHeight();
		OTTERUS_LOG("Loaded Texture: [width = {0}, height = {1} ]", width, height);
		OTTERUS_WARN("Loaded Texture: [width = {0}, height = {1} ]", width, height);

		// Registry from EnTT 
		m_registry = std::make_unique<otterus_core::ECS::Registry>();
		if (!m_registry) {
			OTTERUS_ERROR("Failed to create the EnTT registry");
			return false;
		}

		otterus_core::ECS::Entity entity1{ *m_registry, "Ent1", "Test" };

		auto& transform = entity1.AddComponent<otterus_core::ECS::TransformComponent>(otterus_core::ECS::TransformComponent{
			.position = glm::vec2{10.f, 10.f},
			.scale = glm::vec2{1.f, 1.f},
			.rotation = 0.f
			});

		auto& sprite = entity1.AddComponent<otterus_core::ECS::SpriteComponent>(otterus_core::ECS::SpriteComponent{
		.width = 16.f,
		.height = 16.f,
		.color = otterus_rendering::Color{.r = 0, .g = 255, .b = 255, .a = 255 },
		.start_x = 0,
		.start_y = 0,
			});

		auto& id = entity1.GetComponent<otterus_core::ECS::Identification>();
		OTTERUS_LOG("Entity Name={0}, group={1}, id={2} ", id.name, id.group, id.entity_id);


		sprite.generate_uvs(texture.GetWidth(), texture.GetHeight());
		std::vector<otterus_rendering::Vertex> vertices;
		vertices.reserve(4);

		const float left = transform.position.x;
		const float top = transform.position.y;
		const float right = left + sprite.width * transform.scale.x;
		const float bottom = top + sprite.height * transform.scale.y;

		otterus_rendering::Vertex vTL{
			.position = { left, bottom },
			.uvs = { sprite.uvs.u, sprite.uvs.v + sprite.uvs.uv_height },
			.color = sprite.color
		};

		otterus_rendering::Vertex vTR{
			.position = { left, top },
			.uvs = { sprite.uvs.u, sprite.uvs.v },
			.color = sprite.color
		};

		otterus_rendering::Vertex vBL{
			.position = { right, top },
			.uvs = { sprite.uvs.u + sprite.uvs.uv_width, sprite.uvs.v },
			.color = sprite.color
		};

		otterus_rendering::Vertex vBR{
			.position = { right, bottom },
			.uvs = { sprite.uvs.u + sprite.uvs.uv_width,
					 sprite.uvs.v + sprite.uvs.uv_height },
			.color = sprite.color
		};

		vertices.push_back(vTL);
		vertices.push_back(vTR);
		vertices.push_back(vBL);
		vertices.push_back(vBR);



		GLuint indices[] = {
			0, 1, 3,  // first triangle
			1, 2, 3   // second triangle
		};


		// Create temp camera
		auto camera = std::make_shared<otterus_rendering::Camera2D>();
		camera->SetScale(5.f);

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


		// Generate VAO 
		glGenVertexArrays(1, &VAO);

		// Generate VBO
		glGenBuffers(1, &VBO);

		// Bind VAO, VBO
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(
			GL_ARRAY_BUFFER,											// The target buffer type
			vertices.size() * sizeof(otterus_rendering::Vertex),		// The size in Bytes of the buffer object's new data store
			vertices.data(),											// A pointer to the data that will be copied into data store
			GL_STATIC_DRAW												// Expected usage pattern of data store         
		);

		// Bind IBO
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,                    // The target buffer type
			6 * sizeof(GLuint),							// The size in Bytes of the buffer object's new data store
			indices,									// A pointer to the data that will be copied into data store
			GL_STATIC_DRAW								// Expected usage pattern of data store         
		);


		glVertexAttribPointer(
			0,													 // Attrib 0   -- Layout position in shader source code
			2,													 // Size	    -- Number of compoenent per vertex
			GL_FLOAT,											 // Type       -- Data type of above components
			GL_FALSE,											 // Normalized -- Specifies if fixed-point data values should be normalized
			sizeof(otterus_rendering::Vertex),   				 // Stride     -- Specifies the byte offest between consecutive attributes
			(void*)offsetof(otterus_rendering::Vertex, position) // Pointer    -- Specifies the offset of the first compoenent
		);

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			1,													// Index
			2,													// Size	
			GL_FLOAT,											// Type
			GL_FALSE,											// Normalized
			sizeof(otterus_rendering::Vertex),					// Stride
			(void*)offsetof(otterus_rendering::Vertex, uvs)		// offset to the positional data to the first texture UV coords
		);

		glEnableVertexAttribArray(1);

		glVertexAttribPointer(
			2,													// Index
			4,													// Size	
			GL_UNSIGNED_BYTE,											// Type
			GL_TRUE,											// Normalized
			sizeof(otterus_rendering::Vertex),					// Stride
			(void*)offsetof(otterus_rendering::Vertex, color)		// offset to the positional data to the first texture UV coords
		);

		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
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
	
	}

    void Application::Render()
    {	
		auto& assetManager = m_registry->GetContext<std::shared_ptr<otterus_resources::AssetManager>>();
		auto& camera = m_registry->GetContext<std::shared_ptr<otterus_rendering::Camera2D>>();
		
		auto& shader = assetManager->GetShader("basic");
		auto projection = camera->GetCameraMatrix();

		auto& texture = assetManager->GetTexture("Tile");

		if (shader.GetProgramID() == 0) {
			OTTERUS_ERROR("Failed to Get Basic Shader from AssetManager.");
			return;
		}

		glViewport(
			0,
			0,
			m_window->GetWidth(),
			m_window->GetHeight()
		);

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Enable();

		glBindVertexArray(VAO);

		shader.SetUniformMat4("uProjection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.GetID());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		SDL_GL_SwapWindow(m_window->GetWindow().get());
		shader.Disable();
	}

    void Application::CleanUp()
    {
		SDL_Quit();
	}

    Application::Application():
        m_window{ nullptr }, m_registry{nullptr}, m_event {}, m_isRunning{ true },
        VAO{ 0 }, VBO{ 0 }, IBO{ 0 } // TODO: Remove them
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

