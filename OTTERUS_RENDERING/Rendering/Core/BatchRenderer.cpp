#include "BatchRenderer.h"
#include <algorithm>

namespace otterus_rendering {
	constexpr size_t NUM_SPRITE_VERTICES = 4;
	constexpr size_t NUM_SPRITE_INDICES = 6;
	constexpr size_t MAX_SPRITES = 10000;
	constexpr size_t MAX_INDICES = MAX_SPRITES * NUM_SPRITE_INDICES;
	constexpr size_t MAX_VERTICES = MAX_SPRITES * NUM_SPRITE_VERTICES;

	void BatchRenderer::Initialize()
	{
	
		// Generate VAO 
		glGenVertexArrays(1, &m_VAO);

		// Generate VBO
		glGenBuffers(1, &m_VBO);

		// Bind VAO, VBO
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		
		// Position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		// UVs
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uvs));
		glEnableVertexAttribArray(1);

		// Color
		glVertexAttribPointer( 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(2);

		GLuint offset{ 0 };
		GLuint indices[NUM_SPRITE_INDICES] = {0, 1, 2, 2, 3, 0};

		auto indicesArr = std::make_unique<GLuint[]>(MAX_INDICES);

		for (size_t i = 0; i < MAX_INDICES; i+= NUM_SPRITE_INDICES)
		{
			for (size_t j = 0; j < NUM_SPRITE_INDICES; j++)
			{
				indicesArr[i + j] = indices[j] + offset;
			}
			offset += NUM_SPRITE_VERTICES;

		}

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * MAX_INDICES, indicesArr.get(), GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
	
	}

	void BatchRenderer::GenerateBatches()
	{
		std::vector<Vertex> vertices;
		vertices.resize(m_sprites.size() * NUM_SPRITE_VERTICES);

		int currentVertex{ 0 }, currentSprite{ 0 };
		GLuint prevTextureID{ 0 }, offset{ 0 };

		for (const auto& sprite : m_sprites)
		{
			if (currentSprite == 0)
				m_batches.emplace_back(std::make_shared<Batch>(
					Batch{ .numIndices = NUM_SPRITE_INDICES, .offset = offset, .textureID = sprite->textureID }));

			else if (sprite->textureID != prevTextureID)
				m_batches.emplace_back(std::make_shared<Batch>(
					Batch{ .numIndices = NUM_SPRITE_INDICES, .offset = offset, .textureID = sprite->textureID }));
			else
				m_batches.back()->numIndices += NUM_SPRITE_INDICES;

			vertices[currentVertex++] = sprite->topLeft;
			vertices[currentVertex++] = sprite->topRight;
			vertices[currentVertex++] = sprite->bottomRight;
			vertices[currentVertex++] = sprite->bottomLeft;

			prevTextureID = sprite->textureID;
			offset += NUM_SPRITE_INDICES;
			currentSprite++;

		}

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	BatchRenderer::BatchRenderer():
		m_VAO{ 0 }, m_VBO{ 0 }, m_IBO{0},
		m_sprites{}, m_batches{}
	{
		Initialize();
	}
	BatchRenderer::~BatchRenderer()
	{
		if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
		if (m_IBO) glDeleteBuffers(1, &m_VBO);
		if (m_VBO) glDeleteBuffers(1, &m_IBO);
	}
	void BatchRenderer::Begin()
	{
		m_sprites.clear();
		m_batches.clear();
	}
	void BatchRenderer::End()
	{
		if (m_sprites.empty()) return;

		std::sort(m_sprites.begin(), m_sprites.end(), [&](const auto& a, const auto& b) 
			{
				return a->layer < b->layer;
			}
		);

		GenerateBatches();
	}
	void BatchRenderer::Render()
	{
		if (m_batches.empty()) return;

		glBindVertexArray(m_VAO);

		for (const auto& batch : m_batches)
		{
			glBindTextureUnit(0, batch->textureID);
			glDrawElements(GL_TRIANGLES, batch->numIndices, GL_UNSIGNED_INT, (void*)(sizeof(GLuint)*batch->offset));
		}
		
		glBindVertexArray(0);
	}
	void BatchRenderer::AddSprite(const glm::vec4 & spriteRect, const glm::vec4 uvRect, GLuint textureID, int layer, glm::mat4 model, const Color & color)
	{
		auto newSprite = std::make_shared<Sprite>(
			Sprite{
				.topLeft = Vertex{
					.position = model * glm::vec4{spriteRect.x, spriteRect.y + spriteRect.w, 0.f, 1.f},
					.uvs = glm::vec2{uvRect.x, uvRect.y + uvRect.w},
					.color = color
				},
			.bottomLeft = Vertex{
					.position = model * glm::vec4{spriteRect.x, spriteRect.y, 0.f, 1.f},
					.uvs = glm::vec2{uvRect.x, uvRect.y},
					.color = color
				},
			.topRight = Vertex{
					.position = model * glm::vec4{spriteRect.x + spriteRect.z, spriteRect.y + spriteRect.w, 0.f, 1.f},
					.uvs = glm::vec2{uvRect.x + uvRect.z, uvRect.y + uvRect.w},
					.color = color
				},
			.bottomRight = Vertex{
					.position = model * glm::vec4{spriteRect.x + spriteRect.z, spriteRect.y, 0.f, 1.f},
					.uvs = glm::vec2{uvRect.x + uvRect.w, uvRect.y},
					.color = color
				}
			
			}
		);
	
	}
	
}
