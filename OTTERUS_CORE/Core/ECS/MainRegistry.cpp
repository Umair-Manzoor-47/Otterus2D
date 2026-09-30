#include "MainRegistry.h"
#include "../Resources/AssetManager.h"
#include <Logger/Logger.h>
#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Sounds/SoundPlayer/SoundFXPlayer.h>


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