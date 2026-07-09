#pragma once

#include "../ECS/Registry.h"
#include <Rendering/Core/BatchRenderer.h>


namespace otterus_core::Systems {
	class RenderSystem
	{
	private:
		otterus_core::ECS::Registry& m_registry;
		std::unique_ptr<otterus_rendering::BatchRenderer> m_batchRenderer;

	public:
		RenderSystem(otterus_core::ECS::Registry& registry);
		~RenderSystem() = default;

		void Upate();
	};

}