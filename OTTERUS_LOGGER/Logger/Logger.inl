
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
	void Logger::Log(const std::string& message, Args&&... args) {
		
		assert(m_initialized && "Logger must be initialized before it can be used.");
		
		if (!m_initialized) {
			
			std::cout << "Logger must be initialized before it can be used." << std::endl;
			return;
		}

		std::stringstream ss;
		ss << "OTTERUS [INFO]: " << currentDateTime() << "-" << std::vformat(message, std::make_format_args(args...)) << "\n";
		
		if (m_consoleLog) {

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, GREEN);
			std::cout << ss.str();
			SetConsoleTextAttribute(hConsole, WHITE);
		}

		if (m_retainLogs)
			m_logEntries.emplace_back(LogType::INFO, ss.str());
	
	}


	template <typename... Args>
	void Logger::Warn(const std::string& message, Args&&... args) {

		assert(m_initialized && "Logger must be initialized before it can be used.");

		if (!m_initialized) {

			std::cout << "Logger must be initialized before it can be used." << std::endl;
			return;
		}

		std::stringstream ss;
		ss << "OTTERUS [WARN]: " << currentDateTime() << "-" << std::vformat(message, std::make_format_args(args...)) << "\n";

		if (m_consoleLog) {

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, YELLOW);
			std::cout << ss.str();
			SetConsoleTextAttribute(hConsole, WHITE);
		}

		if (m_retainLogs)
			m_logEntries.emplace_back(LogType::WARN, ss.str());

	
	}


	template <typename... Args>
	void Logger::Error(std::source_location location, const std::string& message, Args&&... args) {
	
		assert(m_initialized && "Logger must be initialized before it can be used.");

		if (!m_initialized) {

			std::cout << "Logger must be initialized before it can be used." << std::endl;
			return;
		}

		std::stringstream ss;
		ss << "OTTERUS [ERROR]: " << currentDateTime() << "-"
			<< std::vformat(message, std::make_format_args(args...))
			<< "\nFUNC: " << location.function_name()
			<< "\nLINE: " << location.line() << "\n\n";

		if (m_consoleLog) {

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, RED);
			std::cout << ss.str();
			SetConsoleTextAttribute(hConsole, WHITE);
		}

		if (m_retainLogs)
			m_logEntries.emplace_back(LogType::ERR, ss.str());
	}

}