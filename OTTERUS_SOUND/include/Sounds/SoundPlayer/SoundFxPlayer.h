#pragma once

namespace otterus_sounds {
	class SoundFxPlayer
	{
	public:
		SoundFxPlayer() = default;
		~SoundFxPlayer() = default;

		/*
		 * @brief Calls Mix_PlayChannel and tries to play the desired soundFx.
		 * Assumes not looping and to play on any open channel.
		 * @param A reference to the SoundFX to play.
		 */
		void Play(class SoundFX& soundFX);

		/*
		 * @brief Calls Mix_PlayChannel and tries to play the desired soundFx.
		 * @param A reference to the SoundFX to play.
		 * @param An int for the number of times the soundFX should loop.
		 * @param An int for the channel to play the sound on. Pass in -1 to play on any open channel.
		 */
		void Play(class SoundFX& soundFX, int loops, int channel = -1);

		/*
		 * @brief Sets the volume for the specified sound channel.
		 * @param Takes in an int value for the volume. The value needs
		 * to be between [0-100] | 0 == MIN | 100 == MAX
		 * @param Takes in an int for the channel we want to set the volume.
		 * [-1] Sets the volume for all channels.
		 */
		void SetVolume(int volume, int channel = -1);

		/*
		 * @brief Halts the sound fx for the specified channel.
		 * @param An int for the desired channel to stop playing.
		 */
		void Stop(int channel);

		/*
		 * @brief Returns true if the specified channel is currently playing.
		 * @param An int for the desired channel to check.
		 */
		bool IsPlaying(int channel);
	};
}