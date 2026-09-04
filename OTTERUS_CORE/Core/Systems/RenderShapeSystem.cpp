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

		if (colorShader.GetProgramID() == 0) {

			OTTERUS_LOG("Color shader program id has not been set correctly");
			return;
		}

		colorShader.Enable();
		colorShader.SetUniformMat4("uProjection", cameraMatrix);
	}
}