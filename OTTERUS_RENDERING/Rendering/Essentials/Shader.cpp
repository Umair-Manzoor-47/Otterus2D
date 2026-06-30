#include "Shader.h"
#include <iostream>
#include <logger/Logger.h>

namespace otterus_rendering {
	
	GLuint Shader::GetUniformLocation(const std::string& uniformName)
	{
		auto uniformItr = m_uniformLocationMap.find(uniformName);
		if (uniformItr != m_uniformLocationMap.end())
			return uniformItr->second;

		GLuint location = glGetUniformLocation(m_shaderProgramID, uniformName.c_str());

		if (location ==  GL_INVALID_INDEX) {

			OTTERUS_ERROR("Uniform name [{0}] not found in shader.", uniformName);
			return -1;
		}

		m_uniformLocationMap.emplace(uniformName, location);

		return location;
	}
	Shader::Shader(GLuint shaderProgram, const std::string& vertexSrcPath, const std::string& fragmentSrcPath) :
		m_shaderProgramID{ shaderProgram }, m_vertexSrcPath{ vertexSrcPath }, m_fragmentSrcPath{fragmentSrcPath} 
	{}

	Shader::~Shader()
	{
		if (m_shaderProgramID > 0) {
		
			glDeleteProgram(m_shaderProgramID);
		}
	}

	void Shader::SetUniformInt(const std::string & name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetUniformMat4(const std::string& name, glm::mat4 mat)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::Enable()
	{
		glUseProgram(m_shaderProgramID);
	}

	void Shader::Disable()
	{
		glUseProgram(0);
	}

}
