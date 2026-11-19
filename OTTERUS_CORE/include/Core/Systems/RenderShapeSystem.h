#pragma once

#include "../ECS/Registry.h"

namespace otterus_rendering
{
	class Camera2D;
	class RectBatchRenderer;
	class CircleBatchRenderer;
}

namespace otterus_core::Systems {
	class RenderShapeSystem
	{
	private:
		std::unique_ptr<otterus_rendering::RectBatchRenderer> m_RectBatchRenderer;
		std::unique_ptr<otterus_rendering::CircleBatchRenderer> m_CircleBatchRenderer;


	public:
		RenderShapeSystem();
		~RenderShapeSystem() = default;

		void Update(ECS::Registry& registry,  otterus_rendering::Camera2D& camera);
	};

}