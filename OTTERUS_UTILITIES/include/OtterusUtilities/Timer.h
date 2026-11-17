#pragma once
#include <chrono>

using namespace std::chrono;
namespace otterus_utils {
	
	class Timer
	{
	private:
		time_point<steady_clock> m_StartPoint, m_PausePoint;
		bool m_IsRunning{ false }, m_IsPaused{ false };

	public:
		Timer();
		~Timer() = default;

		void Start();
		void Pause();
		void Stop();
		void Resume();

		const int64_t ElapsedMS();
		const int64_t ElapsedSec();

		inline const bool IsRunning() const { return m_IsRunning; }
		inline const bool IsPaused() const { return m_IsPaused; }

	};


}