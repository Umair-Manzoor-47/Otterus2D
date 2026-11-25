#include "LightingSystem.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/ECS/Components/LightComponent.h"
#include "Core/ECS/Components/TransformComponent.h"
#include "Core/Resources/AssetManager.h"
#include "Core/CoreUtilities/CoreUtilities.h"

#include <Rendering/Buffers/Framebuffer.h>
#include <Rendering/Core/Camera2D.h>
#include <Rendering/Core/Renderer.h>
#include <Rendering/Essentials/Shader.h>
#include <Logger/Logger.h>

#include <algorithm>
#include <cmath>

using namespace otterus_rendering;
using namespace otterus_core::ECS;

namespace
{
	constexpr int VERTS_PER_QUAD = 6;
	constexpr int FLOATS_PER_VERTEX = 4;
	constexpr int FLOATS_PER_QUAD = VERTS_PER_QUAD * FLOATS_PER_VERTEX;
	constexpr size_t INITIAL_LIGHT_CAPACITY = 32;
}

namespace otterus_core::Systems
{
	LightingSystem::LightingSystem()
		: m_SceneFBO{ std::make_unique<Framebuffer>( 640, 480, false ) }
		, m_LightMapFBO{ std::make_unique<Framebuffer>( 640, 480, false ) }
		, m_LightVBOCapacity{ static_cast<GLsizeiptr>( INITIAL_LIGHT_CAPACITY * FLOATS_PER_QUAD * sizeof( float ) ) }
	{
		m_VisibleLights.reserve( INITIAL_LIGHT_CAPACITY );
		m_LightVertices.reserve( INITIAL_LIGHT_CAPACITY * FLOATS_PER_QUAD );
		InitializeQuads();
	}

	LightingSystem::~LightingSystem()
	{
		if ( m_ScreenVAO != 0 ) glDeleteVertexArrays( 1, &m_ScreenVAO );
		if ( m_ScreenVBO != 0 ) glDeleteBuffers( 1, &m_ScreenVBO );
		if ( m_LightVAO != 0 ) glDeleteVertexArrays( 1, &m_LightVAO );
		if ( m_LightVBO != 0 ) glDeleteBuffers( 1, &m_LightVBO );
	}

	void LightingSystem::CreateQuadBuffer( GLuint& vao, GLuint& vbo, const float* data, GLsizeiptr size, GLenum usage )
	{
		glGenVertexArrays( 1, &vao );
		glGenBuffers( 1, &vbo );

		glBindVertexArray( vao );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, size, data, usage );

		constexpr GLsizei stride = FLOATS_PER_VERTEX * sizeof( float );
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0 );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, stride, (void*)( 2 * sizeof( float ) ) );

		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindVertexArray( 0 );
	}

	void LightingSystem::InitializeQuads()
	{
		constexpr float screenVertices[] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		CreateQuadBuffer( m_ScreenVAO, m_ScreenVBO, screenVertices, sizeof( screenVertices ), GL_STATIC_DRAW );
		CreateQuadBuffer( m_LightVAO, m_LightVBO, nullptr, m_LightVBOCapacity, GL_STREAM_DRAW );
	}

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
		// Clear transparent so empty pixels preserve targetFb's background when composited
		renderer->SetClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		renderer->ClearBuffers( true, false, false );
		renderer->SetBlendCapabilitiy( Renderer::BlendingFactors::SRC_ALPHA, Renderer::BlendingFactors::ONE_MINUS_SRC_ALPHA );
		m_ScenePassActive = true;
	}

	void LightingSystem::GatherVisibleLights( Registry& registry, const Camera2D& camera )
	{
		m_VisibleLights.clear();
		m_LightVertices.clear();

		auto lightView = registry.GetRegistry().view<LightComponent, TransformComponent>();
		for ( const auto& entity : lightView )
		{
			const auto& light = lightView.get<LightComponent>( entity );
			const auto& transform = lightView.get<TransformComponent>( entity );

			const glm::vec4 color = glm::max( light.color, glm::vec4{ 0.0f } );
			const bool hasColor = ( color.r > 0.0f || color.g > 0.0f || color.b > 0.0f ) && color.a > 0.0f;
			if ( light.radius <= 0.0f || light.intensity <= 0.0f || !hasColor )
				continue;

			TransformComponent lightBounds{};
			lightBounds.position = transform.position - glm::vec2{ light.radius };
			const float diameter = light.radius * 2.0f;
			if ( !EntityInView( lightBounds, diameter, diameter, camera ) )
				continue;

			VisibleLight entry{};
			entry.color = color;
			entry.intensity = light.intensity;
			// Clamp falloff > 0 to avoid pow(0, 0) undefined behavior or Inf when negative
			entry.falloff = std::max( light.falloff, 0.01f );
			entry.isSpotlight = light.isSpotlight;

			if ( light.isSpotlight )
			{
				const float rad = glm::radians( transform.rotation );
				const float innerRad = glm::radians( std::min( light.innerAngle, light.outerAngle ) );
				const float outerRad = glm::radians( std::max( light.innerAngle, light.outerAngle ) );

				entry.direction = glm::vec2{ std::cos( rad ), std::sin( rad ) };
				entry.innerCos = std::cos( innerRad );
				entry.outerCos = std::cos( outerRad );
			}

			m_VisibleLights.push_back( entry );

			const float left = transform.position.x - light.radius;
			const float right = transform.position.x + light.radius;
			const float top = transform.position.y - light.radius;
			const float bottom = transform.position.y + light.radius;

			const float quadVerts[FLOATS_PER_QUAD] = {
				left,  top,     0.0f, 0.0f,
				left,  bottom,  0.0f, 1.0f,
				right, bottom,  1.0f, 1.0f,

				left,  top,     0.0f, 0.0f,
				right, bottom,  1.0f, 1.0f,
				right, top,     1.0f, 0.0f
			};

			m_LightVertices.insert( m_LightVertices.end(), std::begin( quadVerts ), std::end( quadVerts ) );
		}
	}

	void LightingSystem::UploadLightQuads()
	{
		const GLsizeiptr requiredBytes = static_cast<GLsizeiptr>( m_LightVertices.size() * sizeof( float ) );

		glBindBuffer( GL_ARRAY_BUFFER, m_LightVBO );
		if ( requiredBytes > m_LightVBOCapacity )
			m_LightVBOCapacity = requiredBytes * 2;

		// Orphan the streaming buffer once per frame to avoid per-light GPU stalls
		glBufferData( GL_ARRAY_BUFFER, m_LightVBOCapacity, nullptr, GL_STREAM_DRAW );
		glBufferSubData( GL_ARRAY_BUFFER, 0, requiredBytes, m_LightVertices.data() );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	void LightingSystem::RenderLightMap( Registry& registry,
										 const Camera2D& camera,
										 const SceneLighting& lighting,
										 Renderer& renderer,
										 Shader* lightShader )
	{
		m_LightMapFBO->Bind();
		renderer.SetViewport( 0, 0, m_LightMapFBO->GetWidth(), m_LightMapFBO->GetHeight() );

		const glm::vec4 ambient = glm::max( lighting.ambientColor, glm::vec4{ 0.0f } );
		renderer.SetClearColor(
			ambient.r * ambient.a,
			ambient.g * ambient.a,
			ambient.b * ambient.a,
			1.0f );
		renderer.ClearBuffers( true, false, false );

		if ( !lightShader || lightShader->GetProgramID() == 0 )
		{
			OTTERUS_ERROR( "LightingSystem: 'light' shader is missing." );
			m_LightMapFBO->Unbind();
			return;
		}

		GatherVisibleLights( registry, camera );
		if ( m_VisibleLights.empty() )
		{
			m_LightMapFBO->Unbind();
			return;
		}

		UploadLightQuads();

		renderer.SetBlendCapabilitiy( Renderer::BlendingFactors::ONE, Renderer::BlendingFactors::ONE );
		lightShader->Enable();
		lightShader->SetUniformMat4( "uProjection", camera.GetCameraMatrix() );

		glBindVertexArray( m_LightVAO );

		for ( size_t i = 0; i < m_VisibleLights.size(); ++i )
		{
			const VisibleLight& light = m_VisibleLights[i];

			lightShader->SetUniformVec4( "uLightColor", light.color );
			lightShader->SetUniformFloat( "uIntensity", light.intensity );
			lightShader->SetUniformFloat( "uFalloff", light.falloff );
			lightShader->SetUniformBool( "uIsSpotlight", light.isSpotlight );

			if ( light.isSpotlight )
			{
				lightShader->SetUniformVec2( "uDirection", light.direction );
				lightShader->SetUniformFloat( "uInnerCos", light.innerCos );
				lightShader->SetUniformFloat( "uOuterCos", light.outerCos );
			}

			glDrawArrays( GL_TRIANGLES, static_cast<GLint>( i * VERTS_PER_QUAD ), VERTS_PER_QUAD );
		}

		glBindVertexArray( 0 );
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

		renderer.SetBlendCapabilitiy( Renderer::BlendingFactors::SRC_ALPHA, Renderer::BlendingFactors::ONE_MINUS_SRC_ALPHA );

		compositeShader->Enable();
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_SceneFBO->GetTextureID() );
		compositeShader->SetUniformInt( "uSceneTexture", 0 );

		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, m_LightMapFBO->GetTextureID() );
		compositeShader->SetUniformInt( "uLightTexture", 1 );

		glBindVertexArray( m_ScreenVAO );
		glDrawArrays( GL_TRIANGLES, 0, VERTS_PER_QUAD );
		glBindVertexArray( 0 );

		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, 0 );
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, 0 );
		compositeShader->Disable();
	}

	void LightingSystem::Update( Framebuffer& targetFb, Registry& registry, Camera2D& camera, const SceneLighting& lighting )
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
