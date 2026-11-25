#include "LightingSystem.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/ECS/Components/LightComponent.h"
#include "Core/ECS/Components/TransformComponent.h"
#include "Core/Resources/AssetManager.h"
#include "Core/CoreUtilities/CoreUtilities.h"

#include <Rendering/Buffers/Framebuffer.h>
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Core/LightBatchRenderer.h>
#include <Rendering/Core/Renderer.h>
#include <Rendering/Essentials/Shader.h>
#include <Logger/Logger.h>

#include <algorithm>
#include <cmath>

using namespace otterus_rendering;
using namespace otterus_core::ECS;

namespace otterus_core::Systems
{
	LightingSystem::LightingSystem()
		: m_SceneFBO{ std::make_unique<Framebuffer>( 640, 480, false ) }
		, m_LightMapFBO{ std::make_unique<Framebuffer>( 640, 480, false ) }
		, m_LightBatchRenderer{ std::make_unique<LightBatchRenderer>() }
	{}

	LightingSystem::~LightingSystem() = default;

	void LightingSystem::EnsureFramebufferSizes( int width, int height )
	{
		if ( width <= 0 || height <= 0 )
			return;

		if ( m_SceneFBO->GetWidth() != width || m_SceneFBO->GetHeight() != height )
			m_SceneFBO->Resize( width, height );
		m_SceneFBO->CheckResize();

		if ( m_LightMapFBO->GetWidth() != width || m_LightMapFBO->GetHeight() != height )
			m_LightMapFBO->Resize( width, height );
		m_LightMapFBO->CheckResize();
	}

	void LightingSystem::BeginScenePass( int width, int height )
	{
		if ( width <= 0 || height <= 0 )
		{
			m_ScenePassActive = false;
			return;
		}

		EnsureFramebufferSizes( width, height );

		auto& renderer = MAIN_REGISTRY().GetContext<std::shared_ptr<Renderer>>();

		m_SceneFBO->Bind();
		renderer->SetViewport( 0, 0, m_SceneFBO->GetWidth(), m_SceneFBO->GetHeight() );
		// Clear transparent so empty pixels preserve targetFb's background when composited,
		// and preserve destination alpha so semi-transparent sprites do not punch holes in opaque ground.
		renderer->SetClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		renderer->ClearBuffers( true, false, false );
		renderer->SetBlendCapabilitySeparate(
			Renderer::BlendingFactors::SRC_ALPHA,
			Renderer::BlendingFactors::ONE_MINUS_SRC_ALPHA,
			Renderer::BlendingFactors::ONE,
			Renderer::BlendingFactors::ONE_MINUS_SRC_ALPHA );
		m_ScenePassActive = true;
	}

	void LightingSystem::RenderLightMap( Registry& registry,
										 const Camera2D& camera,
										 const SceneLighting& lighting,
										 Renderer& renderer,
										 Shader* lightShader )
	{
		m_LightMapFBO->Bind();
		renderer.SetViewport( 0, 0, m_LightMapFBO->GetWidth(), m_LightMapFBO->GetHeight() );

		const float ambientAlpha = lighting.ambientColor.a / 255.0f;
		renderer.SetClearColor(
			( lighting.ambientColor.r / 255.0f ) * ambientAlpha,
			( lighting.ambientColor.g / 255.0f ) * ambientAlpha,
			( lighting.ambientColor.b / 255.0f ) * ambientAlpha,
			1.0f );
		renderer.ClearBuffers( true, false, false );

		if ( !lightShader || lightShader->GetProgramID() == 0 )
		{
			OTTERUS_ERROR( "LightingSystem: 'light' shader is missing." );
			m_LightMapFBO->Unbind();
			return;
		}

		m_LightBatchRenderer->Begin();

		bool hasVisibleLights = false;
		auto lightView = registry.GetRegistry().view<LightComponent, TransformComponent>();
		for ( const auto& entity : lightView )
		{
			const auto& light = lightView.get<LightComponent>( entity );
			const auto& transform = lightView.get<TransformComponent>( entity );

			const bool hasColor = ( light.color.r > 0 || light.color.g > 0 || light.color.b > 0 ) && light.color.a > 0;
			if ( light.radius <= 0.0f || light.intensity <= 0.0f || !hasColor )
				continue;

			const glm::vec2 center = transform.position + light.offset;
			const float diameter = light.radius * 2.0f;

			TransformComponent lightBounds{};
			lightBounds.position = center - glm::vec2{ light.radius };
			if ( !EntityInView( lightBounds, diameter, diameter, camera ) )
				continue;

			// Clamp falloff > 0 to avoid pow(0, 0) undefined behavior or Inf when negative
			const float falloff = std::max( light.falloff, 0.01f );
			glm::vec2 direction{ 1.0f, 0.0f };
			float innerCos{ 1.0f };
			float outerCos{ 1.0f };

			if ( light.isSpotlight )
			{
				const float rad = glm::radians( transform.rotation );
				const float innerRad = glm::radians( std::min( light.innerAngle, light.outerAngle ) );
				const float outerRad = glm::radians( std::max( light.innerAngle, light.outerAngle ) );

				direction = glm::vec2{ std::cos( rad ), std::sin( rad ) };
				innerCos = std::cos( innerRad );
				outerCos = std::cos( outerRad );
			}

			const glm::vec4 destRect{
				center.x - light.radius,
				center.y - light.radius,
				diameter,
				diameter
			};

			m_LightBatchRenderer->AddLight(
				destRect,
				light.color,
				light.intensity,
				falloff,
				light.isSpotlight,
				direction,
				innerCos,
				outerCos );
			hasVisibleLights = true;
		}

		if ( !hasVisibleLights )
		{
			m_LightMapFBO->Unbind();
			return;
		}

		m_LightBatchRenderer->End();

		renderer.SetBlendCapabilitiy( Renderer::BlendingFactors::ONE, Renderer::BlendingFactors::ONE );
		lightShader->Enable();
		lightShader->SetUniformMat4( "uProjection", camera.GetCameraMatrix() );

		m_LightBatchRenderer->Render();

		lightShader->Disable();
		m_LightMapFBO->Unbind();
	}

	void LightingSystem::CompositeToTarget( Framebuffer& targetFb, Renderer& renderer, Shader* compositeShader )
	{
		targetFb.Bind();
		renderer.SetViewport( 0, 0, targetFb.GetWidth(), targetFb.GetHeight() );

		if ( !compositeShader || compositeShader->GetProgramID() == 0 )
		{
			OTTERUS_ERROR( "LightingSystem: 'composite' shader is missing." );
			return;
		}

		renderer.SetBlendCapabilitySeparate(
			Renderer::BlendingFactors::ONE,
			Renderer::BlendingFactors::ONE_MINUS_SRC_ALPHA,
			Renderer::BlendingFactors::ONE,
			Renderer::BlendingFactors::ONE_MINUS_SRC_ALPHA );

		renderer.DrawCompositeQuad( *compositeShader, m_SceneFBO->GetTextureID(), m_LightMapFBO->GetTextureID() );
		renderer.SetBlendCapabilitiy( Renderer::BlendingFactors::SRC_ALPHA, Renderer::BlendingFactors::ONE_MINUS_SRC_ALPHA );
	}

	void LightingSystem::Update( Framebuffer& targetFb, Registry& registry, Camera2D& camera )
	{
		if ( !m_ScenePassActive )
			return;

		m_ScenePassActive = false;
		m_SceneFBO->Unbind();

		if ( targetFb.GetWidth() <= 0 || targetFb.GetHeight() <= 0 )
		{
			targetFb.Bind();
			return;
		}

		const SceneLighting defaultLighting{};
		const SceneLighting& lighting = registry.GetRegistry().ctx().contains<std::shared_ptr<SceneLighting>>()
			? *registry.GetContext<std::shared_ptr<SceneLighting>>()
			: defaultLighting;

		auto& mainRegistry = MAIN_REGISTRY();
		auto& assetManager = mainRegistry.GetContext<std::shared_ptr<otterus_resources::AssetManager>>();
		auto& renderer = mainRegistry.GetContext<std::shared_ptr<Renderer>>();

		const bool wasDepthTestEnabled = renderer->IsCapabilityEnabled( Renderer::GLCapabilities::DEPTH_TEST );
		if ( wasDepthTestEnabled )
			renderer->SetCapability( Renderer::GLCapabilities::DEPTH_TEST, false );

		auto lightShader = assetManager->GetShader( "light" );
		auto compositeShader = assetManager->GetShader( "composite" );

		RenderLightMap( registry, camera, lighting, *renderer, lightShader.get() );
		CompositeToTarget( targetFb, *renderer, compositeShader.get() );

		if ( wasDepthTestEnabled )
			renderer->SetCapability( Renderer::GLCapabilities::DEPTH_TEST, true );
	}
}
