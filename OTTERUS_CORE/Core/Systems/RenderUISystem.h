#pragma once
#include <Rendering/Core/BatchRenderer.h>
#include <Rendering/Core/TextBatchRenderer.h>
#include <Rendering/Core/Camera2D.h>

#include "../ECS/Registry.h"

namespace otterus_core::Systems {
	class RenderUISystem
	{
	private:
		std::unique_ptr<otterus_rendering::SpriteBatchRenderer> m_SpriteRenderer;
		std::unique_ptr<otterus_rendering::TextBatchRenderer> m_TextRenderer;
		std::unique_ptr<otterus_rendering::Camera2D> m_UICamera;

	public:
		RenderUISystem();
		~RenderUISystem() = default;

		void Update(ECS::Registry& registry);
	};

}