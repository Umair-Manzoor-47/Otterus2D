#include "SoundBindings.h"
#include "../ECS/Registry.h"
#include "../Resources/AssetManager.h"
#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Logger/Logger.h>

using namespace otterus_sounds;
using namespace otterus_resources;

void otterus_core::Scripting::SoundBindings::CreateSoundBind(sol::state& lua, otterus_core::ECS::Registry& registry)
{
	auto& musicPlayer = registry.GetContext<std::shared_ptr<MusicPLayer>>();
	if (!musicPlayer) {
		OTTERUS_ERROR("Failed to bind Music Player to lua -- Not in registry.");
		return;
	}

	auto& assetManager = registry.GetContext<std::shared_ptr<AssetManager>>();

	if (!assetManager) {
		OTTERUS_ERROR("Failed to bind Music Player to lua -- AssetManager Not found.");
		return;
	}

	lua.new_usertype<MusicPLayer>(
		"Music",
		sol::no_constructor,
		"play", sol::overload(
			[&](const std::string& musicName, int loops) {
				auto music = assetManager->GetMusic(musicName);
				if (!music) {
					OTTERUS_ERROR("Failed to get music [{}] from AssetManager.");
					return;
				}
				musicPlayer->Play(*music, loops);
			},
			[&](const std::string& musicName) {
				auto music = assetManager->GetMusic(musicName);
				if (!music) {
					OTTERUS_ERROR("Failed to get music [{}] from AssetManager.");
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

}
