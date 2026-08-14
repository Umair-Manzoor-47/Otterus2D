#include "SoundBindings.h"
#include "../ECS/Registry.h"
#include "../Resources/AssetManager.h"
#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Sounds/SoundPlayer/SoundFxPlayer.h>
#include <Logger/Logger.h>

using namespace otterus_sounds;
using namespace otterus_resources;

void otterus_core::Scripting::SoundBinder::CreateSoundBind(sol::state& lua, otterus_core::ECS::Registry& registry)
{
	auto& musicPlayer = registry.GetContext<std::shared_ptr<MusicPlayer>>();
	if (!musicPlayer) {
		OTTERUS_ERROR("Failed to bind Music Player to lua -- Not in registry.");
		return;
	}

	auto& assetManager = registry.GetContext<std::shared_ptr<AssetManager>>();

	if (!assetManager) {
		OTTERUS_ERROR("Failed to bind Music Player to lua -- AssetManager Not found.");
		return;
	}

	lua.new_usertype<MusicPlayer>(
		"Music",
		sol::no_constructor,
		"play", sol::overload(
			[&](const std::string& musicName, int loops) {
				auto music = assetManager->GetMusic(musicName);
				if (!music) {
					OTTERUS_ERROR("Failed to get music [{}] from AssetManager.", musicName);
					return;
				}
				musicPlayer->Play(*music, loops);
			},
			[&](const std::string& musicName) {
				auto music = assetManager->GetMusic(musicName);
				if (!music) {
					OTTERUS_ERROR("Failed to get music [{}] from AssetManager.", musicName);
					return;
				}
				musicPlayer->Play(*music, -1);
			}),
		"stop", [&]() {
			musicPlayer->Stop();
		},
		"pause", [&]() {
			musicPlayer->Pause();
		},
		"resume", [&]() {
			musicPlayer->Resume();
		},
		"set_volume", [&](int volume) {
			musicPlayer->SetVolume(volume);
		},
		"is_playing", [&]() {
			return musicPlayer->IsPlaying();
		}
	);

	auto& soundFxPlayer = registry.GetContext<std::shared_ptr<SoundFxPlayer>>();
	if (!soundFxPlayer) {
		OTTERUS_ERROR("Failed to bind SoundFX Player to lua -- Not in registry.");
		return;
	}
	lua.new_usertype<SoundFxPlayer>(
		"Sound",
		sol::no_constructor,
		"play", sol::overload(
			[&](const std::string& soundFxName, int loops, int channel) {
				auto soundFx = assetManager->GetSoundFX(soundFxName);
				if (!soundFx) {
					OTTERUS_ERROR("Failed to get SoundFx [{}] from AssetManager.", soundFxName);
					return;
				}
				soundFxPlayer->Play(*soundFx, loops, channel);
			},
			[&](const std::string& soundFxName) {
				auto soundFx = assetManager->GetSoundFX(soundFxName);
				if (!soundFx) {
					OTTERUS_ERROR("Failed to get SoundFx [{}] from AssetManager.");
					return;
				}
				soundFxPlayer->Play(*soundFx);
			}),
		"stop", [&](int channel) {
			soundFxPlayer->Stop(channel);
		},
		"set_volume", [&](int volume, int channel) {
			soundFxPlayer->SetVolume(volume, channel);
		},
		"is_playing", [&](int channel) {
			return soundFxPlayer->IsPlaying(channel);
		}
	);

}
