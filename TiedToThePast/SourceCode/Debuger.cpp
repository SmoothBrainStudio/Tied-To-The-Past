#include "Debuger.hpp"

DebugManager::DebugManager()
{
    logsFile.open(PATH_LOGS_FILES);
    logsFile << "";
    logsFile.close();

    logsFile.open(PATH_SYSTEMS_FILES);
    logsFile << "";
    logsFile.close();

    logsFile.open(PATH_WARNINGS_FILES);
    logsFile << "";
    logsFile.close();

    logsFile.open(PATH_ERRORS_FILES);
    logsFile << "";
    logsFile.close();
}

DebugManager& DebugManager::Get()
{
    static DebugManager instance;
    return instance;
}

std::string DebugManager::GetCurrentTime()
{
    std::string result;

    // get the current time
    auto now = std::chrono::system_clock::now();

    // convert the current time to seconds since the epoch
    auto now_seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
    auto value = now_seconds.time_since_epoch().count();

    // convert seconds to hours, minutes, and seconds
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::tm* current_time_tm = std::localtime(&current_time);

    // convert milliseconds
    auto now_milliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto milliseconds = now_milliseconds.time_since_epoch().count() % 1000;

    int hours = current_time_tm->tm_hour;
    int minutes = current_time_tm->tm_min;
    int seconds = current_time_tm->tm_sec;

    // output the current time in the format "hour:minute:second"
    result = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds) + "." + std::to_string(milliseconds);

    return result;
}

void DebugManager::clear() const
{
    std::system(CLEAR);
}