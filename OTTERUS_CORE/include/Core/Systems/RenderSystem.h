#pragma once

#include "../ECS/Registry.h"
#include "OtterusUtilities/HelperUtilities.h"


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
		~RenderSystem();

		void Update( ECS::Registry& registry, otterus_rendering::Camera2D& camera,
			 const std::vector<otterus_utils::SpriteLayerParams>& layerFilters ={} );
	};

}