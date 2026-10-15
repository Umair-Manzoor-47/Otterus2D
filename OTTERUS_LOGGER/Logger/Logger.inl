
#include <chrono>
#include <ctime>
#include <format>
#include <iostream>
#include <windows.h>
#include <sstream>

constexpr WORD GREEN = 2;
constexpr WORD RED = 4;
constexpr WORD YELLOW = 6;
constexpr WORD WHITE = 7;



namespace otterus_logger {

	template <typename... Args>
	void Logger::Log(std::source_location location, const std::string& message, Args&&... args) {
		
		assert(m_Initialized && "Logger must be initialized before it can be used.");
		
		if (!m_Initialized) {
			
			std::cout << "Logger must be initialized before it can be used." << std::endl;
			return;
		}
		std::string formattedMsg = std::vformat(message, std::make_format_args(args...));
		std::string timeStr = CurrentDateTime();
		std::string originStr = FormatOrigin(location);

		if (m_ConsoleLog) {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, GREEN);
			std::cout << "OTTERUS [INFO]: " << timeStr << " - " << formattedMsg << "\n";
			SetConsoleTextAttribute(hConsole, WHITE);
		}

		if (m_RetainLogs) {
			m_LogEntries.push_back(LogEntry{
				.type = LogType::INFO,
				.log = formattedMsg,
				.timestamp = timeStr,
				.origin = originStr,
				.stackTrace = ""
			});
			m_LogAdded = true;
		}
	}


	template <typename... Args>
	void Logger::Warn(std::source_location location, const std::string& message, Args&&... args) {

		assert(m_Initialized && "Logger must be initialized before it can be used.");

		if (!m_Initialized) {

			std::cout << "Logger must be initialized before it can be used." << std::endl;
			return;
		}
		std::string formattedMsg = std::vformat(message, std::make_format_args(args...));
		std::string timeStr = CurrentDateTime();
		std::string originStr = FormatOrigin(location);

		if (m_ConsoleLog) {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, YELLOW);
			std::cout << "OTTERUS [WARN]: " << timeStr << " - " << formattedMsg << "\n";
			SetConsoleTextAttribute(hConsole, WHITE);
		}

		if (m_RetainLogs) {
			m_LogEntries.push_back(LogEntry{
				.type = LogType::WARN,
				.log = formattedMsg,
				.timestamp = timeStr,
				.origin = originStr,
				.stackTrace = ""
			});
			m_LogAdded = true;
		}

	
	}


	template <typename... Args>
	void Logger::Error(std::source_location location, const std::string& message, Args&&... args) {
	
		assert(m_Initialized && "Logger must be initialized before it can be used.");

		if (!m_Initialized) {

			std::cout << "Logger must be initialized before it can be used." << std::endl;
			return;
		}

		std::string formattedMsg = std::vformat(message, std::make_format_args(args...));
		std::string timeStr = CurrentDateTime();
		std::string originStr = FormatOrigin(location);

		if (m_ConsoleLog) {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, RED);
			std::cout << "OTTERUS [ERROR]: " << timeStr << " - " << formattedMsg << "\n";
			SetConsoleTextAttribute(hConsole, WHITE);
		}

		if (m_RetainLogs) {
			m_LogEntries.push_back(LogEntry{
				.type = LogType::ERR,
				.log = formattedMsg,
				.timestamp = timeStr,
				.origin = originStr,
				.stackTrace = ""
			});
			m_LogAdded = true;
		}
	}

}