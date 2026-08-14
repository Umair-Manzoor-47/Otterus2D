#pragma once

namespace otterus_sounds {
	class SoundFxPlayer
	{
	public:
		SoundFxPlayer() = default;
		~SoundFxPlayer() = default;
		void Play(class SoundFX& soundFX);
		void Play(class SoundFX& soundFX, int loops, int channel);
		void Stop(int channel);
		void SetVolume(int channel, int volume);
		bool IsPlaying(int channel);
	};
}