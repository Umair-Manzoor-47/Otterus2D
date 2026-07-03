#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>


namespace otterus_rendering {
	class Shader {
	private:
		GLuint m_shaderProgramID;
		std::string m_vertexSrcPath, m_fragmentSrcPath;
		
		std::unordered_map<std::string, GLuint> m_uniformLocationMap;

		GLuint GetUniformLocation(const std::string& uniformName);

	public:
		Shader();
		Shader(GLuint shaderProgram, const std::string& vertexSrcPath, const std::string& fragmentSrcPath);
		~Shader();

		// TODO: Add Setters as per need
		void SetUniformInt(const std::string& name, int value);
		void SetUniformMat4(const std::string& name, glm::mat4 mat);

		// TODO: Add the Getters for uniform

		void Enable();
		void Disable();


		inline const GLuint GetProgramID() const { return m_shaderProgramID; }

	};


}