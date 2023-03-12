#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <vector>
#include <string_view>
#include <ostream>

enum LogLevel {
    INFO,
    WARN,
    ERR
};

struct LogEntry {
    LogLevel lvl;
    std::string_view message;

};

std::ostream& operator<<(std::ostream& os, const LogEntry& entry);

class Logger {
    static std::vector<LogEntry> messages;

public:
    static void Info(const std::string_view& message);
    static void Warn(const std::string_view& message);
    static void Error(const std::string_view& message);
};

#endif