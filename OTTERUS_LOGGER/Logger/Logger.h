#pragma once

#include <string>
#include <string_view>
#include <source_location>
#include <vector>
#include <cassert>


#if defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL
#define OTTERUS_LOG(x, ...)   otterus_logger::Logger::GetInstance().Log(std::source_location::current(), x, __VA_ARGS__)
#define OTTERUS_WARN(x, ...)  otterus_logger::Logger::GetInstance().Warn(std::source_location::current(), x, __VA_ARGS__)
#define OTTERUS_ERROR(x, ...) otterus_logger::Logger::GetInstance().Error(std::source_location::current(), x, __VA_ARGS__)
#else
#define OTTERUS_LOG(x, ...)   otterus_logger::Logger::GetInstance().Log(std::source_location::current(), x __VA_OPT__(,) __VA_ARGS__)
#define OTTERUS_WARN(x, ...)  otterus_logger::Logger::GetInstance().Warn(std::source_location::current(), x __VA_OPT__(,) __VA_ARGS__)
#define OTTERUS_ERROR(x, ...) otterus_logger::Logger::GetInstance().Error(std::source_location::current(), x __VA_OPT__(,) __VA_ARGS__)
#endif
#define OTTERUS_ASSERT(x) assert(x)
#define OTTERUS_INIT_LOGS(console, retain) otterus_logger::Logger::GetInstance().Init(console, retain)
#define OTTERUS_LOG_ADDED() otterus_logger::Logger::GetInstance().LogAdded()
#define OTTERUS_RESET_ADDED() otterus_logger::Logger::GetInstance().ResetLogAdded()
#define OTTERUS_GET_LOGS() otterus_logger::Logger::GetInstance().GetLogs()
#define OTTERUS_CLEAR_LOGS() otterus_logger::Logger::GetInstance().ClearLogs()

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
		std::string timestamp;
		std::string origin;
		std::string stackTrace;
	};

	class Logger
	{
	private:
		std::vector<LogEntry> m_LogEntries;
		bool m_LogAdded{ false }, m_Initialized{ false }, m_ConsoleLog{ true }, m_RetainLogs{true};

		Logger() = default;

		struct LogTime
		{
			std::string day, dayNumber, month, year, time;
			LogTime(const std::string& date);
		};

		std::string CurrentDateTime();

	public:
		// Since Logger is SINGLETON, we return instance here
		static Logger& GetInstance();


		~Logger() = default;

		// forbid copying -- Logger is [SINGLETON]
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;


		void Init(bool consoleLog = true, bool retainLog = true);

		template <typename... Args>
		void Log(std::source_location location, const std::string& message, Args&&... args);

		template <typename... Args>
		void Warn(std::source_location location, const std::string& message, Args&&... args);

		template <typename... Args>
		void Error(std::source_location location, const std::string& message, Args&&... args);

		void LuaLog(const std::string_view message, const std::string_view origin = "Lua Script");
		void LuaWarn(const std::string_view message, const std::string_view origin = "Lua Script");
		void LuaError(const std::string_view message, const std::string_view origin = "Lua Script");

		inline void ClearLogs() { m_LogEntries.clear(); }

		inline const std::vector<LogEntry>& GetLogs() { return m_LogEntries; }
		inline void ResetLogAdded() { m_LogAdded = false; }
		inline const bool LogAdded() const { return m_LogAdded; }

		std::string FormatOrigin(const std::source_location& loc);
	};
}

#include "Logger.inl"