#include "RenderSystem.h"
#include "../Resources/AssetManager.h"
#include "../ECS/Components/SpriteComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Core/BatchRenderer.h>
#include <Rendering/Essentials/Shader.h>

#include <logger/Logger.h>
#include "Core/ECS/MainRegistry.h"
#include "Core/CoreUtilities/CoreUtilities.h"
#include <ranges>

using namespace otterus_rendering;
using namespace otterus_core::ECS;

namespace otterus_core::Systems {
	RenderSystem::RenderSystem():
		m_batchRenderer{nullptr}
	{
		m_batchRenderer = std::make_unique<SpriteBatchRenderer>();
	}

	RenderSystem::~RenderSystem()
	{}

	void RenderSystem::Update( Registry& registry, Camera2D& camera,
	                           const std::vector<otterus_utils::SpriteLayerParams>& layerFilters )
	{

		auto& assetManager = MAIN_REGISTRY().GetContext<std::shared_ptr<otterus_resources::AssetManager>>();

		auto spriteShader = assetManager->GetShader("basic");
		auto cameraMatrix = camera.GetCameraMatrix();
	
		if (!spriteShader || spriteShader->GetProgramID() == 0) {
		
			OTTERUS_LOG("Sprite shader program id has not been set correctly");
			return;
		}

		spriteShader->Enable();
		spriteShader->SetUniformMat4("uProjection", cameraMatrix);


		m_batchRenderer->Begin();

		auto spriteView = registry.GetRegistry().view<SpriteComponent, TransformComponent>();
		std::function<bool( entt::entity )> filterFunc;

		// Check to see if the layers are visible, if not, filter them out.
		if ( layerFilters.empty() )
		{
			filterFunc = []( entt::entity ) { return true; };
		}
		else
		{
			filterFunc = [ & ]( entt::entity entity ) {
				// We only want to filter tiles
				if ( !registry.GetRegistry().all_of<TileComponent>( entity ) )
					return true;

				const auto& sprite = spriteView.get<SpriteComponent>( entity );
				if ( sprite.layer >= 0 && sprite.layer < layerFilters.size() )
				{
					return layerFilters[ sprite.layer ].visible;
				}

				return false;
			};
		}

		for ( const auto& entity : std::views::filter( spriteView, filterFunc ) )
		{
			const auto& transform = spriteView.get<TransformComponent>(entity);
			const auto& sprite = spriteView.get<SpriteComponent>(entity);

			if (sprite.texture_name.empty()) continue;

			const auto& texture = assetManager->GetTexture(sprite.texture_name);
			GLuint textureID = 0;
			glm::vec4 uvRect{sprite.uvs.u, sprite.uvs.v, sprite.uvs.uv_width, sprite.uvs.uv_height};

			if (!texture || texture->GetID() == 0) {
				auto defaultTex = assetManager->GetDefaultTexture();
				if (!defaultTex || defaultTex->GetID() == 0)
					continue;
				textureID = defaultTex->GetID();
				uvRect = glm::vec4{0.0f, 0.0f, 1.0f, 1.0f};
			} else {
				textureID = texture->GetID();
			}

			glm::vec4 spriteRect{transform.position.x, transform.position.y, sprite.width, sprite.height};
		
			if (!EntityInView(transform, sprite.width, sprite.height, camera))
				continue;
			glm::mat4 model = RSTModel(transform, sprite.width, sprite.height);
			m_batchRenderer->AddSprite(spriteRect, uvRect, textureID, sprite.layer, model, sprite.color);

		}

		m_batchRenderer->End();
		m_batchRenderer->Render();

		spriteShader->Disable();

	}
}