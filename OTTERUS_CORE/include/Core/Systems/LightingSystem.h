#pragma once

#include "Core/ECS/Registry.h"
#include <Rendering/Essentials/SceneLighting.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace otterus_rendering
{
	class Camera2D;
	class Framebuffer;
	class Renderer;
	class Shader;
}

namespace otterus_core::Systems
{

	class LightingSystem
	{
	private:
		struct VisibleLight
		{
			glm::vec4 color{ 1.0f };
			float intensity{ 1.0f };
			float falloff{ 2.0f };
			bool isSpotlight{ false };
			glm::vec2 direction{ 1.0f, 0.0f };
			float innerCos{ 1.0f };
			float outerCos{ 1.0f };
		};

		std::unique_ptr<otterus_rendering::Framebuffer> m_SceneFBO;
		std::unique_ptr<otterus_rendering::Framebuffer> m_LightMapFBO;

		GLuint m_ScreenVAO{ 0 }, m_ScreenVBO{ 0 };
		GLuint m_LightVAO{ 0 }, m_LightVBO{ 0 };
		GLsizeiptr m_LightVBOCapacity{ 0 };

		bool m_ScenePassActive{ false };

		// Reused across frames to avoid per-frame heap allocations
		std::vector<VisibleLight> m_VisibleLights;
		std::vector<float> m_LightVertices;

		void InitializeQuads();
		void CreateQuadBuffer( GLuint& vao, GLuint& vbo, const float* data, GLsizeiptr size, GLenum usage );
		void EnsureFramebufferSizes( int width, int height );
		void GatherVisibleLights( ECS::Registry& registry, const otterus_rendering::Camera2D& camera );
		void UploadLightQuads();

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
					 otterus_rendering::Camera2D& camera,
					 const otterus_rendering::SceneLighting& lighting );
	};
}
