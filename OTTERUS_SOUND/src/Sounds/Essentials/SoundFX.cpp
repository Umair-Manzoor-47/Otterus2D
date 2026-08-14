#include "SoundFX.h"

otterus_sounds::SoundFX::SoundFX(const SoundParams& params, SoundFxPtr soundFX)
	: m_soundParams{ params }, m_soundFX{ std::move(soundFX) }
{}
