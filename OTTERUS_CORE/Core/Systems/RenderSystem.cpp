#include "RenderSystem.h"
#include "../Resources/AssetManager.h"
#include "../ECS/Components/SpriteComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Essentials/Shader.h>

#include <logger/Logger.h>


using namespace otterus_rendering;
using namespace otterus_core::ECS;

namespace otterus_core::Systems {
	RenderSystem::RenderSystem(Registry& registry):
		m_registry{ registry }, m_batchRenderer{nullptr}
	{
		m_batchRenderer = std::make_unique<otterus_rendering::BatchRenderer>();
	}
	void RenderSystem::Upate()
	{
		auto& camera = m_registry.GetContext<std::shared_ptr<Camera2D>>();
		auto& assetManager = m_registry.GetContext<std::shared_ptr<otterus_resources::AssetManager>>();

		auto& spriteShader = assetManager->GetShader("basic");
		auto cameraMatrix = camera->GetCameraMatrix();
	
		if (spriteShader.GetProgramID() == 0) {
		
			OTTERUS_LOG("Sprite shader program id has not been set correctly");
			return;
		}

		spriteShader.Enable();
		spriteShader.SetUniformMat4("uProjection", cameraMatrix);


		m_batchRenderer->Begin();
		
		auto view = m_registry.GetRegistry().view<SpriteComponent, TransformComponent>();

		for (const auto entity : view)
		{
			const auto& transform = view.get<TransformComponent>(entity);
			const auto& sprite = view.get<SpriteComponent>(entity);

			if (sprite.texture_name.empty()) continue;

			const auto& texture = assetManager->GetTexture(sprite.texture_name);

			if (texture.GetID() == 0) {
			
				OTTERUS_ERROR("texture [{0}] was not created correctly.", sprite.texture_name);
				return;
			}

			glm::vec4 spriteRect{transform.position.x, transform.position.y, sprite.width, sprite.height};
			glm::vec4 uvRect{sprite.uvs.u, sprite.uvs.v, sprite.uvs.uv_width, sprite.uvs.uv_height};
		
			glm::mat4 model{1.f};

			if (transform.rotation > 0.f || transform.rotation < 0.f || 
				transform.scale.x > 1.f || transform.scale.x < 1.f || 
				transform.scale.y > 1.f || transform.scale.y < 1.f ) 
			{
				model = glm::translate(model, glm::vec3{ transform.position, 0.f });
				model = glm::translate(model, glm::vec3{ sprite.width * 0.5f, sprite.height * 0.5f, 0.f });


				model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3{ 0.f, 0.f, 1.f });

				model = glm::translate(model, glm::vec3{ sprite.width * -0.5f, sprite.height * -0.5f, 0.f });
				model = glm::scale(model, glm::vec3{ transform.scale, 1.f });
				model = glm::translate(model, glm::vec3{ -transform.position, 0.f });

			}

			m_batchRenderer->AddSprite(spriteRect, uvRect, texture.GetID(), sprite.layer, model, sprite.color);

		}

		m_batchRenderer->End();
		m_batchRenderer->Render();

		spriteShader.Disable();

	}
}