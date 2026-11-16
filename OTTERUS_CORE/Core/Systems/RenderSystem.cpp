#include "RenderSystem.h"
#include "../Resources/AssetManager.h"
#include "../ECS/Components/SpriteComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Essentials/Shader.h>

#include <logger/Logger.h>
#include "Core/ECS/MainRegistry.h"
#include "Core/CoreUtilities/CoreUtilities.h"

using namespace otterus_rendering;
using namespace otterus_core::ECS;

namespace otterus_core::Systems {
	RenderSystem::RenderSystem():
		m_batchRenderer{nullptr}
	{
		m_batchRenderer = std::make_unique<otterus_rendering::SpriteBatchRenderer>();
	}
	void RenderSystem::Update(Registry& registry)
	{
		auto& camera = registry.GetContext<std::shared_ptr<Camera2D>>();

		auto& assetManager = MAIN_REGISTRY().GetContext<std::shared_ptr<otterus_resources::AssetManager>>();

		auto& spriteShader = assetManager->GetShader("basic");
		auto cameraMatrix = camera->GetCameraMatrix();
	
		if (spriteShader.GetProgramID() == 0) {
		
			OTTERUS_LOG("Sprite shader program id has not been set correctly");
			return;
		}

		spriteShader.Enable();
		spriteShader.SetUniformMat4("uProjection", cameraMatrix);


		m_batchRenderer->Begin();
		
		auto view = registry.GetRegistry().view<SpriteComponent, TransformComponent>();

		for (const auto entity : view)
		{
			const auto& transform = view.get<TransformComponent>(entity);
			const auto& sprite = view.get<SpriteComponent>(entity);

			if (sprite.texture_name.empty()) continue;

			const auto& texture = assetManager->GetTexture(sprite.texture_name);

			if (texture->GetID() == 0) {
			
				OTTERUS_ERROR("texture [{0}] was not created correctly.", sprite.texture_name);
				return;
			}

			glm::vec4 spriteRect{transform.position.x, transform.position.y, sprite.width, sprite.height};
			glm::vec4 uvRect{sprite.uvs.u, sprite.uvs.v, sprite.uvs.uv_width, sprite.uvs.uv_height};
		
			if (!EntityInView(transform, sprite.width, sprite.height, *camera))
				continue;
			glm::mat4 model = RSTModel(transform, sprite.width, sprite.height);
			m_batchRenderer->AddSprite(spriteRect, uvRect, texture->GetID(), sprite.layer, model, sprite.color);

		}

		m_batchRenderer->End();
		m_batchRenderer->Render();

		spriteShader.Disable();

	}
}