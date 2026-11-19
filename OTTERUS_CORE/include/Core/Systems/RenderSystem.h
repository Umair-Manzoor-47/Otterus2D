#pragma once

#include "../ECS/Registry.h"

namespace otterus_rendering
{
	class Camera2D;
	class SpriteBatchRenderer;
}

namespace otterus_core::Systems {
	class RenderSystem
	{
	private:
		std::unique_ptr<otterus_rendering::SpriteBatchRenderer> m_batchRenderer;

	public:
		RenderSystem();
		~RenderSystem() = default;

		void Update(ECS::Registry& registry, otterus_rendering::Camera2D& camera);
	};

}