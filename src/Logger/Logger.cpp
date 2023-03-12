#include <iomanip>
#include <iostream>
#include <chrono>
/*
available from gcc13 onwards, using strftime until then
#include <format>
*/
#include "Logger.hpp"

auto getCurrentTime() -> std::string {
    auto t = std::time({});
    std::string timeString(20, '\0');
    std::strftime(std::data(timeString), std::size(timeString), "%d-%m-%Y %H:%M:%S", std::localtime(&t));
    return timeString;
}

std::ostream& operator<<(std::ostream& os, const LogEntry& entry) {
    switch(entry.lvl) {
        case LogLevel::INFO : os << "\033[1;32m INFO | "; break;
        case LogLevel::WARN : os << "\033[1;33m WARN | "; break;
        case LogLevel::ERR  : os << "\033[1;31m ERR | " ; break;
        default: break;
    }
    os << getCurrentTime() << " | " << entry.message << "\n";
    return os;
}

void Logger::Info(const std::string_view& msg) {
    LogEntry logEntry{LogLevel::INFO, msg};
    std::cout << logEntry;
}

void Logger::Warn(const std::string_view& msg) {
    LogEntry logEntry{LogLevel::WARN, msg};
    std::cout << logEntry;
}

void Logger::Error(const std::string_view& msg) {
    LogEntry logEntry{LogLevel::ERR, msg};
    std::cout << logEntry;
}