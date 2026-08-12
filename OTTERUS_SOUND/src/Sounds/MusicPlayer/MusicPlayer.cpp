#include "MusicPlayer.h"
#include "../Essentials/Music.h"
#include <Logger.h>

namespace otterus_sounds {
    MusicPLayer::MusicPLayer()
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) 
        {
            std::string error{ Mix_GetError() };
            OTTERUS_ERROR("Unable to open SDL_Mixer - {}", error);
            return;
        }
        OTTERUS_LOG("Channels Allocated [{}] ", Mix_AllocateChannels(16));
    }

    MusicPLayer::~MusicPLayer()
    {
        Mix_HaltMusic();
        Mix_Quit();
        OTTERUS_LOG("Music Player closed");
    }

    void MusicPLayer::Play(Music& music, int loops)
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

    void MusicPLayer::Pause()
    {}

    void MusicPLayer::Resume()
    {}

    void MusicPLayer::Stop()
    {}

    void MusicPLayer::SetVolume(int volume)
    {}

    bool MusicPLayer::IsPlaying()
    {
        return false;
    }
}