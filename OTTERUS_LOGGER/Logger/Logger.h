#pragma once

#include <string>
#include <string_view>
#include <source_location>
#include <vector>
#include <cassert>


#define OTTERUS_LOG(x, ...) otterus_logger::Logger::GetInstance().Log(x, __VA_ARGS__);
#define OTTERUS_WARN(x, ...) otterus_logger::Logger::GetInstance().Warn(x, __VA_ARGS__);
#define OTTERUS_ERROR(x, ...) otterus_logger::Logger::GetInstance().Error(std::source_location::current(), x, __VA_ARGS__);
#define OTTERUS_INIT_LOGS(console, retain) otterus_logger::Logger::GetInstance().Init(console, retain);

namespace otterus_logger {

	enum class LogType {INFO, WARN, ERR, NONE};

	/// <summary>
	/// LogEntry is useful object when logging onto 
	/// different systems like custom console, system console or
	/// a log file
	/// </summary>
	struct LogEntry
	{
		LogType type{LogType::INFO};
		std::string log{""};
	};

	class Logger
	{
	private:
		std::vector<LogEntry> m_logEntries;
		bool m_logAdded{ false }, m_initialized{ false }, m_consoleLog{ true }, m_retainLogs{true};

		Logger() = default;

		struct LogTime
		{
			std::string day, dayNumber, month, year, time;
			LogTime(const std::string& date);
		};

		std::string currentDateTime();

	public:
		// Since Logger is SINGLETON, we return instance here
		static Logger& GetInstance();


		~Logger() = default;

		// forbid copying -- Logger is [SINGLETON]
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;


		void Init(bool consoleLog = true, bool retainLog = true);

		template <typename... Args>
		void Log(const std::string& message, Args&&... args);


		template <typename... Args>
		void Warn(const std::string& message, Args&&... args);


		template <typename... Args>
		void Error(std::source_location location, const std::string& message, Args&&... args);

	};
}

#include "Logger.inl"