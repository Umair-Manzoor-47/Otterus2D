#pragma once
#include "Vertex.h"

namespace otterus_rendering {
	struct Batch
	{
		GLuint numIndices{ 0 }, offset{ 0 }, textureID{ 0 };

	};

	struct LineBatch {
		GLuint offset{ 2 }, numVertices{ 0 };
		float lineWidth{1.f};
	};

	struct RectBatch
	{
		GLuint numIndices{ 0 }, offset{ 0 };
	};

	struct RectGlyph
	{
		Vertex topLeft, bottomLeft, topRight, bottomRight;
	};

	struct CircleGlyph
	{
		CircleVertex topLeft, bottomLeft, topRight, bottomRight;
	};

	struct SpriteGlyph
	{
		Vertex topLeft, bottomLeft, topRight, bottomRight;
		int layer;
		GLuint textureID;
	};

	struct LineGlyph
	{
		Vertex p1, p2;
		float lineWidth;
	};
}