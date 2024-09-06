#ifndef DEBUGER_HPP
#define DEBUGER_HPP

#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <type_traits>
#include <list>
#ifdef _WIN32
static constexpr auto CLEAR = "cls";
#else
static constexpr auto CLEAR "clear"
#endif

static constexpr auto COLOR_RED = "\x1b[31m";
static constexpr auto COLOR_GREEN = "\x1b[32m";
static constexpr auto COLOR_YELLOW = "\x1b[33m";
static constexpr auto COLOR_BLUE = "\x1b[34m";
static constexpr auto COLOR_MAGENTA = "\x1b[35m";
static constexpr auto COLOR_CYAN = "\x1b[36m";
static constexpr auto COLOR_GRAY = "\x1b[90m";
static constexpr auto COLOR_RESET = "\x1b[0m";

static constexpr auto _LOG = true;
static constexpr auto _WARNING = true;
static constexpr auto _ERROR = true;
static constexpr auto _SYSTEM = true;

static constexpr auto PATH_LOGS_FILES = "Contents/Data/Logs/logs.txt";
static constexpr auto PATH_WARNINGS_FILES = "Contents/Data/Logs/warnings.txt";
static constexpr auto PATH_ERRORS_FILES = "Contents/Data/Logs/errors.txt";
static constexpr auto PATH_SYSTEMS_FILES = "Contents/Data/Logs/systems.txt";

template<class T>
struct is_debug_array
{
	static constexpr bool value = false;
};

template<class T, class Alloc>
struct is_debug_array<std::vector<T, Alloc>>
{
	static constexpr bool value = true;
};

template<class T, std::size_t N>
struct is_debug_array<std::array<T, N>>
{
	static constexpr bool value = true;
};

template<class T>
struct is_debug_array<std::list<T>>
{
	static constexpr bool value = true;
};

class DebugManager
{
private:
	DebugManager();
	~DebugManager() = default;
	DebugManager(const DebugManager&) = delete;
	DebugManager& operator=(const DebugManager&) = delete;

	bool IsOkToWrite;
	std::ofstream logsFile;

public:
	static DebugManager& Get();

	std::string GetCurrentTime();
	void clear() const;

	template<class T>
	void Write(const std::string& _debugName, const std::string& _debugColor, const std::string& _debugPath, const T& value)
	{
#if _DEBUG
		if (!IsOkToWrite)
		{
			logsFile.open(_debugPath, std::ios::app);
			logsFile << _debugColor << "<" << _debugName << ", " << GetCurrentTime() << "> \t";

			std::cout << _debugColor << "<" << _debugName << ", " << GetCurrentTime() << "> \t";
		}

		if constexpr (is_debug_array<T>::value)
		{
			std::cout << "{ ";
			logsFile << "{ ";

			for (auto it = std::begin(value); it != std::end(value); ++it)
			{
				std::cout << *it;
				logsFile << *it;

				if (std::next(it) != std::end(value))
				{
					std::cout << "; ";
					logsFile << "; ";
				}
			}

			std::cout << " }";
			logsFile << " }";
		}
		else
		{
			std::cout << value;
			logsFile << value;
		}

		logsFile << "\n";
		logsFile.close();
		std::cout << COLOR_RESET << std::endl;
		IsOkToWrite = false;
#endif
	}

	template <class T, class ...Args>
	void Write(const std::string& _debugName, const std::string& _debugColor, const std::string& _debugPath, const T& value, const Args&... args)
	{
#if _DEBUG
		if (!IsOkToWrite)
		{
			logsFile.open(_debugPath, std::ios::app);
			logsFile << _debugColor << "<" << _debugName << ", " << GetCurrentTime() << "> \t";

			std::cout << _debugColor << "<" << _debugName << ", " << GetCurrentTime() << "> \t";
			IsOkToWrite = true;
		}

		if constexpr (is_debug_array<T>::value)
		{
			std::cout << "{ ";
			logsFile << "{ ";

			for (auto it = std::begin(value); it != std::end(value); ++it)
			{
				std::cout << *it;
				logsFile << *it;

				if (std::next(it) != std::end(value))
				{
					std::cout << "; ";
					logsFile << "; ";
				}
			}

			std::cout << " }";
			logsFile << " }";
		}
		else
		{
			std::cout << value;
			logsFile << value;
		}

		Write(_debugName, _debugColor, _debugPath, args...);
#endif
	}

	template<class T>
	void log(const T& value)
	{
		Write("LOG", COLOR_RESET, PATH_LOGS_FILES, value);
	}
	template<class T, class ...Args>
	void log(const T& value, const Args&... args)
	{
		Write("LOG", COLOR_RESET, PATH_LOGS_FILES, value, args...);
	}

	template<class T>
	void warning(const T& value)
	{
		Write("WARNING", COLOR_YELLOW, PATH_WARNINGS_FILES, value);
	}
	template<class T, class ...Args>
	void warning(const T& value, const Args&... args)
	{
		Write("WARNING", COLOR_YELLOW, PATH_WARNINGS_FILES, value, args...);
	}

	template<class T>
	void error(const T& value)
	{
		Write("ERROR", COLOR_RED, PATH_ERRORS_FILES, value);
		throw;
	}
	template<class T, class ...Args>
	void error(const T& value, const Args&... args)
	{
		Write("ERROR", COLOR_RED, PATH_ERRORS_FILES, value, args...);
	}

	template<class T>
	void system(const T& value)
	{
		Write("SYSTEM", COLOR_GRAY, PATH_SYSTEMS_FILES, value);
	}
	template<class T, class ...Args>
	void system(const T& value, const Args&... args)
	{
		Write("SYSTEM", COLOR_GRAY, PATH_SYSTEMS_FILES, value, args...);
	}


};

#endif // !DEBUGER_HPP