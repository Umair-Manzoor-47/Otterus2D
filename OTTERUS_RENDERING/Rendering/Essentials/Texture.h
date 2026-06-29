#pragma once
#include <glad/glad.h>

namespace otterus_rendering {
	// struct for Texture

	struct uvs {
		float u, v, width, height;

		uvs() :
			uvs(0, 0, 0, 0)
		{}

		uvs(float u, float v, float width, float height) :
			u{ u }, v{ v }, width{ width }, height{ height }
		{};
	};

	class Texture {
	private:
		GLuint m_texID;
		int m_width, m_height;


	public:
		Texture(GLuint texID, int width, int height);

		inline GLuint GetID() const { return m_texID; }
		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
	};

}