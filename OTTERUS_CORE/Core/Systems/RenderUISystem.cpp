#include "RenderUISystem.h"
#include "../Resources/AssetManager.h"
#include "../ECS/Components/TextComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include <Rendering/Essentials/Font.h>
#include <Logger/Logger.h>

using namespace otterus_core::ECS;
using namespace otterus_resources;

namespace otterus_core::Systems {
	RenderUISystem::RenderUISystem(Registry& registry)
		: m_registry{ registry },
		m_SpriteRenderer { std::make_unique<otterus_rendering::SpriteBatchRenderer>() },
		m_TextRenderer{ std::make_unique<otterus_rendering::TextBatchRenderer>() },
		m_UICamera{ nullptr }

	{
		m_UICamera = std::make_unique<otterus_rendering::Camera2D>(640, 480);
		m_UICamera->Update();
	}

	void RenderUISystem::Upate(entt::registry& registry)
	{
		auto& assetManager = m_registry.GetContext<std::shared_ptr<AssetManager>>();

		auto& fontShader = assetManager->GetShader("font");
		if (fontShader.GetProgramID() == 0) {

			OTTERUS_LOG("Failed to get font shader from AssetManager.");
			return;
		}
		auto cameraMatrix = m_UICamera->GetCameraMatrix();

		auto textView = m_registry.GetRegistry().view<TextComponent, TransformComponent>();


		fontShader.Enable();
		fontShader.SetUniformMat4("uProjection", cameraMatrix);


		m_TextRenderer->Begin();


		for (const auto entity : textView)
		{
			const auto& text = textView.get<TextComponent>(entity);
			if (text.isHidden || text.fontName.empty())
				continue;

			const auto& font = assetManager->GetFont(text.fontName);

			if (!font) {
				OTTERUS_ERROR("Failed to get font [{0}] from asset manager.", text.fontName);
				continue;
			}

			const auto& transform = textView.get<TransformComponent>(entity);

			glm::mat4 model{ 1.f };

			if (transform.rotation > 0.f || transform.rotation < 0.f ||
				transform.scale.x > 1.f || transform.scale.x < 1.f ||
				transform.scale.y > 1.f || transform.scale.y < 1.f)
			{
				model = glm::translate(model, glm::vec3{ transform.position, 0.f });
				model = glm::translate(model, glm::vec3{ (font->GetFontSize() * transform.scale.x) * 0.5f, (font->GetFontSize() * transform.scale.y) * 0.5f, 0.f });


				model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3{ 0.f, 0.f, 1.f });

				model = glm::translate(model, glm::vec3{ (font->GetFontSize() * transform.scale.x) * -0.5f, (font->GetFontSize() * transform.scale.y) * -0.5f, 0.f });
				model = glm::scale(model, glm::vec3{ transform.scale, 1.f });
				model = glm::translate(model, glm::vec3{ -transform.position, 0.f });

			}

			m_TextRenderer->AddText(text.textStr, font, transform.position, text.padding, text.wrap, text.color, model);

		}

		m_TextRenderer->End();
		m_TextRenderer->Render();

		fontShader.Disable();
	}
}