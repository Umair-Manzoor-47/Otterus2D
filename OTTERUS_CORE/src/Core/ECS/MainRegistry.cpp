#include "MainRegistry.h"
#include "../Resources/AssetManager.h"
#include <Logger/Logger.h>
#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Sounds/SoundPlayer/SoundFXPlayer.h>

#include "AnimationSystem.h"
#include "LightingSystem.h"
#include "PhysicsSystem.h"
#include "RenderShapeSystem.h"
#include "RenderSystem.h"
#include "RenderUISystem.h"
#include "ScriptingSystem.h"
#include "Rendering/Core/Renderer.h"


namespace otterus_core::ECS
{

	MainRegistry& MainRegistry::GetInstance()
	{
		static MainRegistry instance{};
		return instance;
	}

	void MainRegistry::Initialize()
	{
		m_MainRegistry = std::make_unique<Registry>();
		OTTERUS_ASSERT(m_MainRegistry && "Failed to initialize main registry.");

		auto assetManager = std::make_shared<otterus_resources::AssetManager>();
		m_MainRegistry->AddToContext<std::shared_ptr<otterus_resources::AssetManager>>(std::move(assetManager));

		auto musicPlayer = std::make_shared<otterus_sounds::MusicPlayer>();
		m_MainRegistry->AddToContext<std::shared_ptr<otterus_sounds::MusicPlayer>>(std::move(musicPlayer));

		auto soundPlayer = std::make_shared<otterus_sounds::SoundFxPlayer>();
		m_MainRegistry->AddToContext<std::shared_ptr<otterus_sounds::SoundFxPlayer>>(std::move(soundPlayer));

		auto renderer = std::make_shared<otterus_rendering::Renderer>();
		// Enable Blending
		renderer->SetBlendCapabilitiy(otterus_rendering::Renderer::BlendingFactors::SRC_ALPHA, otterus_rendering::Renderer::BlendingFactors::ONE_MINUS_SRC_ALPHA);
		// Add Renderer to Registry
		m_MainRegistry->AddToContext<std::shared_ptr<otterus_rendering::Renderer>>(std::move(renderer));

		auto scriptingSystem = std::make_shared<Systems::ScriptingSystem>();
		m_MainRegistry->AddToContext<std::shared_ptr<Systems::ScriptingSystem>>(std::move(scriptingSystem));

		auto renderSystem = std::make_shared<Systems::RenderSystem>();
		m_MainRegistry->AddToContext<std::shared_ptr<Systems::RenderSystem>>(std::move(renderSystem));

		auto lightingSystem = std::make_shared<Systems::LightingSystem>();
		m_MainRegistry->AddToContext<std::shared_ptr<Systems::LightingSystem>>(std::move(lightingSystem));

		auto renderShapeSystem = std::make_shared<Systems::RenderShapeSystem>();
		m_MainRegistry->AddToContext<std::shared_ptr<Systems::RenderShapeSystem>>(std::move(renderShapeSystem));

		auto renderUISystem = std::make_shared<Systems::RenderUISystem>();
		m_MainRegistry->AddToContext<std::shared_ptr<Systems::RenderUISystem>>(std::move(renderUISystem));

		auto animationSystem = std::make_shared<Systems::AnimationSystem>();
		m_MainRegistry->AddToContext<std::shared_ptr<Systems::AnimationSystem>>(std::move(animationSystem));

		auto physicsSystem = std::make_shared <Systems::PhysicsSystem>();
		m_MainRegistry->AddToContext<std::shared_ptr<Systems::PhysicsSystem>>(std::move(physicsSystem));

		m_Initialized = true;
	}

	otterus_resources::AssetManager& MainRegistry::GetAssetManager()
	{
		OTTERUS_ASSERT(m_Initialized && "Main Registry must be initialized before use.");
		return *m_MainRegistry->GetContext<std::shared_ptr<otterus_resources::AssetManager>>();
	}

	otterus_sounds::MusicPlayer& MainRegistry::GetMusicPlayer()
	{
		OTTERUS_ASSERT(m_Initialized && "Main Registry must be initialized before use.");
		return *m_MainRegistry->GetContext<std::shared_ptr<otterus_sounds::MusicPlayer>>();
	}

	otterus_sounds::SoundFxPlayer& MainRegistry::GetSoundPlayer()
	{
		OTTERUS_ASSERT(m_Initialized && "Main Registry must be initialized before use.");
		return *m_MainRegistry->GetContext<std::shared_ptr<otterus_sounds::SoundFxPlayer>>();
	}

}
