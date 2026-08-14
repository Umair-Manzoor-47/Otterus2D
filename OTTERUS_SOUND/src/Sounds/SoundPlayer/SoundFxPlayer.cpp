#include "SoundFxPlayer.h"
#include "../Essentials/SoundFX.h"
#include <Logger.h>

namespace otterus_sounds {
	
	void SoundFxPlayer::Play(class SoundFX& soundFX)
	{
		Play(soundFX, 0, -1);
	}
	void SoundFxPlayer::Play(class SoundFX& soundFX, int loops, int channel)
	{
		if (!soundFX.GetSoundFXPtr()) {
			OTTERUS_ERROR("Failed to play soundfx [{}] on channel [{}] : Invalid sound pointer.", soundFX.GetName(), channel);
			return;
		}

		if (Mix_PlayChannel(channel, soundFX.GetSoundFXPtr(), loops) == -1) {
		
			std::string error{ Mix_GetError() };
			OTTERUS_ERROR("Failed to play soundfx [{}] on channel [{}] : MixError: [{}]", soundFX.GetName(), channel, error);

		}
	}
	
	void SoundFxPlayer::Stop(int channel)
	{
		if(Mix_HaltChannel(channel) == -1) {

			std::string error{ Mix_GetError() };
			OTTERUS_ERROR("Failed to HALT channel [{}] : MixError: [{}]", channel == -1? "all channels" : std::to_string(channel), error);
		}
	}

	void SoundFxPlayer::SetVolume(int channel, int volume)
	{
		if (volume < 0 || volume > 100)
		{
			OTTERUS_ERROR("Invalid volume level. Please provide a value between 0 and 100.");
			return;
		}
		int volumechanged = static_cast<int>((volume / 100) * 128);
		Mix_Volume(channel, volumechanged);
	
	}
	bool SoundFxPlayer::IsPlaying(int channel)
	{
		return Mix_Playing(channel);
	}
}
