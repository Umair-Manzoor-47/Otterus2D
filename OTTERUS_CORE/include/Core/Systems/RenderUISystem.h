#pragma once
#include "../ECS/Registry.h"

namespace otterus_rendering
{
	class Camera2D;
	class SpriteBatchRenderer;
	class TextBatchRenderer;
}

namespace otterus_core::Systems {
	class RenderUISystem
	{
	private:
		std::unique_ptr<otterus_rendering::SpriteBatchRenderer> m_SpriteRenderer;
		std::unique_ptr<otterus_rendering::TextBatchRenderer> m_TextRenderer;
		std::unique_ptr<otterus_rendering::Camera2D> m_UICamera;

	public:
		RenderUISystem();
		~RenderUISystem();

		void Update(ECS::Registry& registry);
	};

}