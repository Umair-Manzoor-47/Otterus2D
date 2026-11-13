#pragma once

#include "../ECS/Registry.h"
#include <Rendering/Core/BatchRenderer.h>


namespace otterus_core::Systems {
	class RenderSystem
	{
	private:
		std::unique_ptr<otterus_rendering::SpriteBatchRenderer> m_batchRenderer;

	public:
		RenderSystem();
		~RenderSystem() = default;

		void Update(ECS::Registry& registry);
	};

}