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

	// Temporary Vertices
	//float vertices[] = {
	//	0.0f, 0.5f, 0.f,
	//	-0.5f, -0.5f, 0.f,
	//	0.5f, -0.5f, 0.f,
	//};

	// Quad vertices
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top-right
		 0.5f, -0.5f, 0.0f,  // bottom-right
		-0.5f, -0.5f, 0.0f,  // bottom-left
		-0.5f,  0.5f, 0.0f   // top-left
	};

	GLuint indices[] = {
		0, 1, 3,  // first triangle
		1, 2, 3   // second triangle
	};

	// Create temp vertices source
	const char* vertexShaderSource =
		"#version 450 core\n"
		"layout (location = 0) in vec3 aPosition;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(aPosition, 1.0);\n"
		"}\0";

	// Create shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Set source
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	
	// Compile Vertex Shader
	glCompileShader(vertexShader);

	// Get status
	int status;

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	if (!status) {
		
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Failed to compile vertex shader, " << infoLog << std::endl;
		return -1;
	}

	// Create temp fragment source
	const char* fragmentShaderSource =
		"#version 450 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
		"}\0";

	// Create shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Set source
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// Compile Fragment Shader
	glCompileShader(fragmentShader);

	// Get status
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

	if (!status) {

		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Failed to compile fragment shader, " << infoLog << std::endl;
		return -1;
	}

	// Create shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	// Attach shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Link the program after attaching
	glLinkProgram(shaderProgram);

	// Check Link status
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (!status) {

		char infoLog[512];
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Failed to link shader program, " << infoLog << std::endl;
		return -1;
	}

	// Now Shader Program can be enabled
	glUseProgram(shaderProgram);

	// once shader program is linked we don't really need vertex and fragment shaders, delete them
	glDeleteProgram(vertexShader);
	glDeleteProgram(fragmentShader);


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
		GL_ARRAY_BUFFER,                          // The target buffer type
		sizeof(vertices) * 3 * sizeof(float),     // The size in Bytes of the buffer object's new data store
		vertices,                                 // A pointer to the data that will be copied into data store
		GL_STATIC_DRAW                            // Expected usage pattern of data store         
		);

	// Bind IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,                          // The target buffer type
		6 * sizeof(GLuint),     // The size in Bytes of the buffer object's new data store
		indices,                                 // A pointer to the data that will be copied into data store
		GL_STATIC_DRAW                            // Expected usage pattern of data store         
	);




	glVertexAttribPointer(
		0,                        // Attrib 0   -- Layout position in shader source code
		3,						  // Size	    -- Number of compoenent per vertex
		GL_FLOAT,                 // Type       -- Data type of above components
		GL_FALSE,                 // Normalized -- Specifies if fixed-point data values should be normalized
		3 * sizeof(float),   	  // Stride     -- Specifies the byte offest between consecutive attributes
		(void*)0                  // Pointer    -- Specifies the offset of the first compoenent
	);

	glEnableVertexAttribArray(0);
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

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		SDL_GL_SwapWindow(window.GetWindow().get());
	}

	std::cout << "EXIT" << std::endl;

	return 0;
}