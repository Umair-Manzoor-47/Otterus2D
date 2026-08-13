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
    {}

    void MusicPlayer::Resume()
    {}

    void MusicPlayer::Stop()
    {}

    void MusicPlayer::SetVolume(int volume)
    {}

    bool MusicPlayer::IsPlaying()
    {
        return false;
    }
}