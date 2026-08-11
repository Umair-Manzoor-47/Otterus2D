#pragma once


namespace otterus_sounds {

	class MusicPLayer
	{
	public:
		MusicPLayer();
		~MusicPLayer();
		
		void Play(class Music& music, int loops = 0);
		void Pause();
		void Resume();
		void Stop();
		void SetVolume(int volume);
		bool IsPlaying();
	};
}