#pragma once

#include "Batcher.h"
#include "../Essentials/BatchTypes.h"

namespace otterus_rendering
{
	class LightBatchRenderer : public Batcher<LightBatch, LightGlyph>
	{
	private:
		virtual void GenerateBatches() override;
		void Initialize();

	public:
		LightBatchRenderer();
		~LightBatchRenderer() = default;

		virtual void End() override;
		virtual void Render() override;

		void AddLight( const glm::vec4& destRect,
					   const Color& color,
					   float intensity,
					   float falloff,
					   bool isSpotlight = false,
					   const glm::vec2& direction = glm::vec2{ 1.f, 0.f },
					   float innerCos = 1.f,
					   float outerCos = 1.f );
	};
}
