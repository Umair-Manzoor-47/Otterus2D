#include "Music.h"

otterus_sounds::Music::Music(const SoundParams& params, MusicPtr pMusic)
	: m_Params{ params }, m_Music{std::move(pMusic)}
{}
