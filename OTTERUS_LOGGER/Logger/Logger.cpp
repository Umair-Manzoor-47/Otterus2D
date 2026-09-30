#include "Logger.h"


namespace otterus_logger {
	Logger::LogTime::LogTime(const std::string& date) :
		day{ date.substr(0, 3) }, dayNumber{ date.substr(8, 2) }, month{ date.substr(4, 3) }
		, year{ date.substr(20, 4) }, time{date.substr(11, 8)}
	{
	
	}

	std::string Logger::CurrentDateTime()
	{
		auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		char buf[30];
		ctime_s(buf, sizeof(buf), &time);

		LogTime logTime{ std::string{buf} };
	
		return std::format("{0}-{1}-{2} {3}", logTime.year, logTime.month, logTime.dayNumber, logTime.time);
	}

	Logger& Logger::GetInstance()
	{
		static Logger instance{};
		return instance;
	}
	void Logger::Init(bool consoleLog, bool retainLog)
	{
		assert(!m_Initialized && "Do not initialize more than once.");
		if (m_Initialized)
		{
			std::cout << "Logger already initialized." << std::endl;
			return;
		}
		
		m_ConsoleLog = consoleLog;
		m_RetainLogs = retainLog;
		m_Initialized = true;
	
	}
	void Logger::LuaLog(const std::string_view message)
	{
		assert(m_Initialized && "The logger must be initialized before it is used!");

		if (!m_Initialized)
		{
			std::cout << "The logger must be initialized before it is used!" << std::endl;
			return;
		}

		std::stringstream ss;
		ss << "LUA [INFO]: " << CurrentDateTime() << " - " << message << "\n";

		if ( m_ConsoleLog)
		{
#ifdef _WIN32
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, GREEN);
			std::cout << ss.str();
			SetConsoleTextAttribute(hConsole, WHITE);
#else
			std::cout << GREEN << ss.str() << CLOSE << "\n\n";
#endif
		}

		if (m_RetainLogs)
		{
			m_LogEntries.emplace_back(LogType::INFO, ss.str());
			m_LogAdded = true;
		}
	}

	void Logger::LuaWarn(const std::string_view message)
	{
		assert(m_Initialized && "The logger must be initialized before it is used!");

		if (!m_Initialized)
		{
			std::cout << "The logger must be initialized before it is used!" << std::endl;
			return;
		}

		std::stringstream ss;
		ss << "LUA [WARN]: " << CurrentDateTime() << " - " << message << "\n";

		if ( m_ConsoleLog)
		{
#ifdef _WIN32
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, YELLOW);
			std::cout << ss.str();
			SetConsoleTextAttribute(hConsole, WHITE);
#else
			std::cout << YELLOW << ss.str() << CLOSE << "\n\n";
#endif
		}

		if (m_RetainLogs)
		{
			m_LogEntries.emplace_back(LogType::WARN, ss.str());
			m_LogAdded = true;
		}
	}

	void Logger::LuaError(const std::string_view message)
	{
		assert(m_Initialized && "The logger must be initialized before it is used!");

		if (!m_Initialized)
		{
			std::cout << "The logger must be initialized before it is used!" << std::endl;
			return;
		}

		std::stringstream ss;
		ss << "LUA [ERROR]: " << CurrentDateTime() << " - " << message << "\n";

		if ( m_ConsoleLog)
		{
#ifdef _WIN32
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, RED);
			std::cout << ss.str();
			SetConsoleTextAttribute(hConsole, WHITE);
#else
			std::cout << RED << ss.str() << CLOSE << "\n\n";
#endif
		}

		if (m_RetainLogs)
		{
			m_LogEntries.emplace_back(LogType::ERR, ss.str());
			m_LogAdded = true;
		}
	}
}


