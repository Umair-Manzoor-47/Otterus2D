#pragma once

#include "Core/ECS/Registry.h"
#include <Rendering/Essentials/SceneLighting.h>
#include <memory>

namespace otterus_rendering
{
	class Camera2D;
	class Framebuffer;
	class LightBatchRenderer;
	class Renderer;
	class Shader;
}

namespace otterus_core::Systems
{
	class LightingSystem
	{
	private:
		std::unique_ptr<otterus_rendering::Framebuffer> m_SceneFBO;
		std::unique_ptr<otterus_rendering::Framebuffer> m_LightMapFBO;
		std::unique_ptr<otterus_rendering::LightBatchRenderer> m_LightBatchRenderer;

		bool m_ScenePassActive{ false };

		void EnsureFramebufferSizes( int width, int height );
		void RenderLightMap( ECS::Registry& registry,
							 const otterus_rendering::Camera2D& camera,
							 const otterus_rendering::SceneLighting& lighting,
							 otterus_rendering::Renderer& renderer,
							 otterus_rendering::Shader* lightShader );
		void CompositeToTarget( otterus_rendering::Framebuffer& targetFb,
								otterus_rendering::Renderer& renderer,
								otterus_rendering::Shader* compositeShader );

	public:
		LightingSystem();
		~LightingSystem();

		LightingSystem( const LightingSystem& ) = delete;
		LightingSystem& operator=( const LightingSystem& ) = delete;

		void BeginScenePass( int width, int height );
		void Update( otterus_rendering::Framebuffer& targetFb,
					 ECS::Registry& registry,
					 otterus_rendering::Camera2D& camera );
	};
}
