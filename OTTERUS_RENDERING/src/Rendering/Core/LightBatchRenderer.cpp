#include "LightBatchRenderer.h"

namespace otterus_rendering
{
	void LightBatchRenderer::GenerateBatches()
	{
		std::vector<LightVertex> vertices;
		vertices.resize( m_Glyphs.size() * NUM_SPRITE_VERTICES );

		int currentVertex{ 0 }, currentLight{ 0 };
		GLuint offset{ 0 };

		for ( const auto& light : m_Glyphs )
		{
			if ( currentLight == 0 )
			{
				m_Batches.emplace_back( std::make_shared<LightBatch>(
					LightBatch{ .numIndices = NUM_SPRITE_INDICES, .offset = offset } ) );
			}
			else
			{
				m_Batches.back()->numIndices += NUM_SPRITE_INDICES;
			}

			vertices[currentVertex++] = light->topLeft;
			vertices[currentVertex++] = light->topRight;
			vertices[currentVertex++] = light->bottomRight;
			vertices[currentVertex++] = light->bottomLeft;

			offset += NUM_SPRITE_INDICES;
			currentLight++;
		}

		glBindBuffer( GL_ARRAY_BUFFER, GetVBO() );
		glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( LightVertex ), nullptr, GL_DYNAMIC_DRAW );
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size() * sizeof( LightVertex ), vertices.data() );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	void LightBatchRenderer::Initialize()
	{
		SetVertexAttribute( 0, 2, GL_FLOAT, sizeof( LightVertex ), (void*)offsetof( LightVertex, position ) );
		SetVertexAttribute( 1, 2, GL_FLOAT, sizeof( LightVertex ), (void*)offsetof( LightVertex, uvs ) );
		SetVertexAttribute( 2, 4, GL_UNSIGNED_BYTE, sizeof( LightVertex ), (void*)offsetof( LightVertex, color ), GL_TRUE );
		SetVertexAttribute( 3, 1, GL_FLOAT, sizeof( LightVertex ), (void*)offsetof( LightVertex, intensity ) );
		SetVertexAttribute( 4, 1, GL_FLOAT, sizeof( LightVertex ), (void*)offsetof( LightVertex, falloff ) );
		SetVertexAttribute( 5, 2, GL_FLOAT, sizeof( LightVertex ), (void*)offsetof( LightVertex, direction ) );
		SetVertexAttribute( 6, 1, GL_FLOAT, sizeof( LightVertex ), (void*)offsetof( LightVertex, innerCos ) );
		SetVertexAttribute( 7, 1, GL_FLOAT, sizeof( LightVertex ), (void*)offsetof( LightVertex, outerCos ) );
		SetVertexAttribute( 8, 1, GL_FLOAT, sizeof( LightVertex ), (void*)offsetof( LightVertex, isSpotlight ) );
	}

	LightBatchRenderer::LightBatchRenderer()
		: Batcher{ true }
	{
		Initialize();
	}

	void LightBatchRenderer::End()
	{
		if ( m_Glyphs.empty() )
			return;

		GenerateBatches();
	}

	void LightBatchRenderer::Render()
	{
		if ( m_Batches.empty() )
			return;

		EnableVAO();

		for ( const auto& batch : m_Batches )
		{
			glDrawElements( GL_TRIANGLES, batch->numIndices, GL_UNSIGNED_INT, (void*)( sizeof( GLuint ) * batch->offset ) );
		}

		DisableVAO();
	}

	void LightBatchRenderer::AddLight( const glm::vec4& destRect,
									   const Color& color,
									   float intensity,
									   float falloff,
									   bool isSpotlight,
									   const glm::vec2& direction,
									   float innerCos,
									   float outerCos )
	{
		const float spotFlag = isSpotlight ? 1.0f : 0.0f;

		auto makeVert = [&]( float px, float py, float u, float v ) {
			return LightVertex{
				.position = glm::vec2{ px, py },
				.uvs = glm::vec2{ u, v },
				.color = color,
				.intensity = intensity,
				.falloff = falloff,
				.direction = direction,
				.innerCos = innerCos,
				.outerCos = outerCos,
				.isSpotlight = spotFlag
			};
		};

		auto newLight = std::make_shared<LightGlyph>(
			LightGlyph{
				.topLeft = makeVert( destRect.x, destRect.y + destRect.w, 0.0f, 1.0f ),
				.bottomLeft = makeVert( destRect.x, destRect.y, 0.0f, 0.0f ),
				.topRight = makeVert( destRect.x + destRect.z, destRect.y + destRect.w, 1.0f, 1.0f ),
				.bottomRight = makeVert( destRect.x + destRect.z, destRect.y, 1.0f, 0.0f )
			} );

		m_Glyphs.emplace_back( std::move( newLight ) );
	}
}
