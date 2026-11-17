#pragma once
#include <OtterusUtilities/SDL_wrappers.h>
#include "SoundParams.h"

namespace otterus_sounds {
	class SoundFX
	{
	private:
		SoundParams m_soundParams;
		SoundFxPtr m_soundFX;

	public:
		SoundFX(const SoundParams& params, SoundFxPtr soundFX);
		~SoundFX() = default;

		inline const std::string& GetName() const { return m_soundParams.name; }
		inline const std::string& GetFilename() const { return m_soundParams.filename; }
		inline const std::string& GetDescription() const { return m_soundParams.description; }
		inline const double GetDuration() const { return m_soundParams.duration; }
		inline Mix_Chunk* GetSoundFXPtr() const { if (!m_soundFX) return nullptr; return m_soundFX.get(); }
	};
}