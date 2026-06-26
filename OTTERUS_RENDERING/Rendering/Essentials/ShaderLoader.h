#pragma once

#include "Shader.h"
#include <memory>

namespace otterus_rendering {

	class ShaderLoader
	{
	private:
		static GLuint createProgram(const std::string& vertexShader, const std::string& fragmentShader);
		static GLuint compileShader(GLuint shaderType, const std::string& filepath);
		static bool compileSuccess(GLuint shader);
		static bool isProgramValid(GLuint program);
		static bool linkShaders(GLuint program, GLuint vertexShader, GLuint fragmentShader);

	public:
		ShaderLoader() = delete;
		static std::shared_ptr<Shader> Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		
	};

}