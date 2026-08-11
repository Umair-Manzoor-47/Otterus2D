#include "MusicPlayer.h"
#include "../Essentials/Music.h"
#include <Logger.h>

namespace otterus_sounds {
    MusicPLayer::MusicPLayer()
    {}

    MusicPLayer::~MusicPLayer()
    {}

    void MusicPLayer::Play(Music& music, int loops)
    {}

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