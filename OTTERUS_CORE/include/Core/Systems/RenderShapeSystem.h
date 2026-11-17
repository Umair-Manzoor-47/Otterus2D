#pragma once

#include "../ECS/Registry.h"
#include <Rendering/Core/RectBatchRenderer.h>
#include <Rendering/Core/CircleBatchRenderer.h>


namespace otterus_core::Systems {
	class RenderShapeSystem
	{
	private:
		std::unique_ptr<otterus_rendering::RectBatchRenderer> m_RectBatchRenderer;
		std::unique_ptr<otterus_rendering::CircleBatchRenderer> m_CircleBatchRenderer;


	public:
		RenderShapeSystem();
		~RenderShapeSystem() = default;

		void Update(ECS::Registry& registry);
	};

}