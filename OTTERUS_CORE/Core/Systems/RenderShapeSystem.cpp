#include "RenderShapeSystem.h"
#include "../ECS/Components/BoxColliderComponent.h"
#include "../ECS/Components/CircleColliderComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include "../Resources/AssetManager.h"
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Essentials/Primitives.h>
#include <Logger/Logger.h>

using namespace otterus_core::ECS;
using namespace otterus_rendering;
using namespace otterus_resources;
namespace otterus_core::Systems {
	RenderShapeSystem::RenderShapeSystem(otterus_core::ECS::Registry& registry)
		: m_registry{ registry },
		m_RectBatchRenderer{ std::make_unique<RectBatchRenderer>() },
		m_CircleBatchRenderer{ std::make_unique<CircleBatchRenderer>() }
	{}

	void RenderShapeSystem::Upate()
	{
		auto& camera = m_registry.GetContext<std::shared_ptr<Camera2D>>();
		auto& assetManager = m_registry.GetContext<std::shared_ptr<otterus_resources::AssetManager>>();

		auto& colorShader = assetManager->GetShader("color");
		auto cameraMatrix = camera->GetCameraMatrix();

		colorShader.Enable();
		colorShader.SetUniformMat4("projection", cameraMatrix);
		m_RectBatchRenderer->Begin();

		auto boxView = m_registry.GetRegistry().view<TransformComponent, BoxColliderComponent>();
		for (auto entity : boxView)
		{
			auto& transform = boxView.get<TransformComponent>(entity);
			auto& boxCollider = boxView.get<BoxColliderComponent>(entity);

			glm::mat4 model{ 1.f };

			if (transform.rotation > 0.f || transform.rotation < 0.f ||
				transform.scale.x > 1.f || transform.scale.x < 1.f ||
				transform.scale.y > 1.f || transform.scale.y < 1.f)
			{
				model = glm::translate(model, glm::vec3{ transform.position, 0.f });
				model = glm::translate(model, glm::vec3{ boxCollider.width * 0.5f, boxCollider.height * 0.5f, 0.f });


				model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3{ 0.f, 0.f, 1.f });

				model = glm::translate(model, glm::vec3{ boxCollider.width * -0.5f, boxCollider.height * -0.5f, 0.f });
				model = glm::scale(model, glm::vec3{ transform.scale, 1.f });
				model = glm::translate(model, glm::vec3{ -transform.position, 0.f });

			}

			Rect rect{
				.position = glm::vec2{ 
					transform.position.x + boxCollider.offset.x,
					transform.position.y + boxCollider.offset.y
				},
				.width = transform.scale.x * boxCollider.width,
				.height = transform.scale.y * boxCollider.height,
				.color = Color{255, 0, 0, 135}
			};

			m_RectBatchRenderer->AddRect(rect, model);
		}
		m_RectBatchRenderer->End();
		m_RectBatchRenderer->Render();
		colorShader.Disable();


		// Circle
		auto& circleShader = assetManager->GetShader("circle");

		circleShader.Enable();
		circleShader.SetUniformMat4("uProjection", cameraMatrix);
		m_CircleBatchRenderer->Begin();

		auto circleView = m_registry.GetRegistry().view<TransformComponent, CircleColliderComponent>();
		for (auto entity : circleView)
		{
			auto& transform = circleView.get<TransformComponent>(entity);
			auto& circleCollider = circleView.get<CircleColliderComponent>(entity);

			glm::vec4 circle{
				transform.position.x + circleCollider.offset.x,
				transform.position.y + circleCollider.offset.y,
				circleCollider.radius * transform.scale.x * 2,
				circleCollider.radius* transform.scale.y * 2
			};

			m_CircleBatchRenderer->AddCircle(circle, Color{0, 255, 0, 135}, 1.f);
		}
		m_CircleBatchRenderer->End();
		m_CircleBatchRenderer->Render();
		circleShader.Disable();
	}
}