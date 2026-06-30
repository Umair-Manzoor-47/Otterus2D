#include "ShaderLoader.h"
#include <iostream>
#include <fstream>
#include <logger/Logger.h>

namespace otterus_rendering{


    GLuint ShaderLoader::createProgram(const std::string& vertexShader, const std::string& fragmentShader)
    {
        const GLuint program = glCreateProgram();

        const GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexShader);
        const GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

        if (vertex == 0 || fragment == 0) return 0;

        if (!linkShaders(program, vertex, fragment)) {
            OTTERUS_ERROR("Failed to link shader.");
            return 0;
        }

        return program;
    }
    GLuint ShaderLoader::compileShader(GLuint shaderType, const std::string& filepath)
    {
        const GLuint shaderID = glCreateShader(shaderType);

        std::ifstream ifs(filepath);

        if(ifs.fail()){
        
            OTTERUS_ERROR("Failed to open shader file [{0}] .", filepath);
            return 0;
        }

        std::string contents{""};
        std::string line;

        while(std::getline(ifs, line))
        {
            contents += line + "\n";
        
        }

        ifs.close();

        const char* contentsPtr = contents.c_str();
        glShaderSource(shaderID, 1, &contentsPtr, nullptr);

        glCompileShader(shaderID);

        if (!compileSuccess(shaderID)) {
            
            OTTERUS_ERROR("Failed to compile shader [{0}] .", filepath);
            return 0;
        }


        return shaderID;
    }

    bool ShaderLoader::compileSuccess(GLuint shader)
    {
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE) {

            GLint maxLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::string errorLog(maxLength, ' ');

            glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog.data());
            OTTERUS_ERROR("Failed to compile shader, {0}" , std::string{ errorLog } );
            
            glDeleteShader(shader);
            
            return false;
        }
        return true;
    }
    bool ShaderLoader::isProgramValid(GLuint program)
    {
        GLint status;

        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (status != GL_TRUE) {

            GLint maxLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::string errorLog(maxLength, ' ');
            glGetProgramInfoLog(program, maxLength, &maxLength, errorLog.data());
            std::cout << "Shader's Program failed to link, " << std::string{ errorLog } << std::endl;
            
            return false;
        }


        return true;
    }
    bool ShaderLoader::linkShaders(GLuint program, GLuint vertexShader, GLuint fragmentShader)
    {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        if (!isProgramValid(program)) {
            glDeleteProgram(program);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            
            return false;
        }

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);


        return true;
    }
    std::shared_ptr<Shader> ShaderLoader::Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        GLuint program = createProgram(vertexShaderPath, fragmentShaderPath);
        if (program)
            return std::make_shared<Shader>(program, vertexShaderPath, fragmentShaderPath);

        return nullptr;
    }
}
