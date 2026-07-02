#define SDL_MAIN_HANDLED 1;
#define NOMINMAX
#include <windowing/window/window.h>
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Rendering/Essentials/ShaderLoader.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Essentials/Vertex.h>
#include <entt.hpp>

#include <Logger/Logger.h>


// Temporary will move it to sprite

struct UVs {
	float u{ 0.f }, v{ 0.f }, uv_width{ 0.f }, uv_height{ 0.f };
};

struct TransformComponent {

	glm::vec2 position{ glm::vec2{0.f} }, scale{ glm::vec2{1.f} };
	float rotation{0.f};
};

struct SpriteComponent {
	float width{ 0.f }, height{ 0.f };
	
	UVs uvs{.u=0.f, .v = 0.f, .uv_width = 0.f, .uv_height = 0.f};

	otterus_rendering::Color color{ .r = 255, .g = 255, .b = 255, .a = 255 };

	int start_x{0}, start_y{0};

	void generate_uvs(float textureWidth, float textureHeight)
	{
		uvs.uv_width = width / textureWidth;
		uvs.uv_height = height / textureHeight;

		uvs.u = start_x * uvs.uv_width;
		uvs.v = start_y * uvs.uv_height;
	};

};

int main() {

	OTTERUS_INIT_LOGS(true, true);

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


	// initialize Glad
	if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
	{
		std::cout << "Failed to load openGL --> GLAD" << std::endl;
	
		running = false;

		return -1;
	}

	// Enable Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Registry from EnTT 
	auto pRegistry = std::make_unique<entt::registry>();
	if (!pRegistry) {
		OTTERUS_ERROR("Failed to create the EnTT registry");
		return -1;
	}



	// Create temp texture
	auto texture = otterus_rendering::TextureLoader::Create("assets/textures/tiles.png", otterus_rendering::Texture::TextureType::BLENDED);
	
	if (!texture) {
		OTTERUS_ERROR("Failed to load texture.");
		return -1;
	}

	int width = texture->GetWidth();
	int height = texture->GetHeight();
	OTTERUS_LOG("Loaded Texture: [width = {0}, height = {1} ]", width, height);
	OTTERUS_WARN("Loaded Texture: [width = {0}, height = {1} ]", width, height);

	UVs UVS;
	
	// Create Entities
	auto ent1 = pRegistry->create();

	auto& transform = pRegistry->emplace<TransformComponent>(ent1, TransformComponent{
		.position = glm::vec2{10.f, 10.f},
		.scale = glm::vec2{1.f, 1.f},
		.rotation = 0.f
		});

	auto& sprite = pRegistry->emplace<SpriteComponent>(ent1, SpriteComponent{
	.width = 16.f,
	.height = 16.f,
	.color = otterus_rendering::Color{.r = 0, .g = 255, .b = 255, .a = 255 },
	.start_x = 0,
	.start_y = 0,
	});

	sprite.generate_uvs(texture->GetWidth(), texture->GetHeight());
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
	otterus_rendering::Camera2D camera{};
	camera.SetScale(5.f);


	auto shader = otterus_rendering::ShaderLoader::Create("assets/shaders/basic_shader.vert", "assets/shaders/basic_shader.frag");

	if (!shader) {
		std::cout << "Failed to create the shader." << std::endl;
		return -1;
	}

	// Create Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
	GLuint VAO, VBO, IBO;


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

	// Window Loop
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
			0,
			0,
			window.GetWidth(),
			window.GetHeight()
		);

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->Enable();

		glBindVertexArray(VAO);

		auto projection = camera.GetCameraMatrix();
		shader->SetUniformMat4("uProjection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetID());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		SDL_GL_SwapWindow(window.GetWindow().get());
		camera.Update();
		shader->Disable();
	}

	std::cout << "EXIT" << std::endl;

	return 0;
}