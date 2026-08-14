#include "MusicPlayer.h"
#include "../Essentials/Music.h"
#include <Logger.h>

namespace otterus_sounds {
    MusicPlayer::MusicPlayer()
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) 
        {
            std::string error{ Mix_GetError() };
            OTTERUS_ERROR("Unable to open SDL_Mixer - {}", error);
            return;
        }
        OTTERUS_LOG("Channels Allocated [{}] ", Mix_AllocateChannels(16));
    }

    MusicPlayer::~MusicPlayer()
    {
        Mix_HaltMusic();
        Mix_Quit();
        OTTERUS_LOG("Music Player closed");
    }

    void MusicPlayer::Play(Music& music, int loops)
    {
        if (!music.GetMusicPtr()) {
            OTTERUS_ERROR("Failed to play music [{}] - Mix music was null.", music.GetName());
            return;
        }

        if (Mix_PlayMusic(music.GetMusicPtr(), loops) != 0) {
            std::string error{ Mix_GetError() };
            OTTERUS_ERROR("Failed to play music [{}] - Mix error: {}", music.GetName(), error);
        }
    }

    void MusicPlayer::Pause()
    {
        Mix_PauseMusic();
    }

    void MusicPlayer::Resume()
    {
        Mix_ResumeMusic();
    }

    void MusicPlayer::Stop()
    {
        Mix_HaltMusic();
    }

    void MusicPlayer::SetVolume(int volume)
    {
        if(volume < 0 || volume > 100)
        {
            OTTERUS_ERROR("Invalid volume level. Please provide a value between 0 and 100.");
            return;
        }
        int volumechanged = static_cast<int>((volume/100) * 128);
        Mix_VolumeMusic(volumechanged);
    }

    bool MusicPlayer::IsPlaying()
    {

        return Mix_PlayingMusic();
    }
}