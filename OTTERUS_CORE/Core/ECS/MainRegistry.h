#pragma once
#include <memory>
#include <Logger/Logger.h>
#include "Registry.h"

#define MAIN_REGISTRY() otterus_core::ECS::MainRegistry::GetInstance()

namespace otterus_resources
{
	class AssetManager;
}
namespace otterus_sounds
{
	class MusicPlayer;
	class SoundFxPlayer;
}

namespace otterus_core::ECS
{
	class MainRegistry
	{
	private:
		std::unique_ptr<Registry> m_MainRegistry{ nullptr };
		bool m_Initialized{ false };

		MainRegistry() = default;
		~MainRegistry() = default;
		MainRegistry(const MainRegistry&) = delete;
		MainRegistry& operator=(const MainRegistry&) = delete;

	public:
		static MainRegistry& GetInstance();
		void Initialize();

		otterus_resources::AssetManager& GetAssetManager();
		otterus_sounds::MusicPlayer& GetMusicPlayer();
		otterus_sounds::SoundFxPlayer& GetSoundPlayer();

		template <typename TContext>
		TContext AddToContext(TContext context)
		{
			OTTERUS_ASSERT(m_Initialized && "Main Registry must be initialized before use.");
			return m_MainRegistry->AddToContext<TContext>(context);
		}

		template <typename TContext>
		TContext& GetContext()
		{
			OTTERUS_ASSERT(m_Initialized && "Main Registry must be initialized before use.");
			return m_MainRegistry->GetContext<TContext>();
		}
	};
}