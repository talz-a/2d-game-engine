#include "Logger.hpp"

#include <chrono>
#include <iostream>
#include <print>

constexpr auto GREEN_COLOR = "\e[0;32m";
constexpr auto RESET_COLOR = "\e[0m";
constexpr auto RED_COLOR = "\e[0;31m";

void Logger::Log(const std::string& message) {
    std::string logString = std::format("LOG: [{}] - {}", std::chrono::system_clock::now(), message);
    LogEntry logEntry = {
        .type = LogType::LOG_INFO,
        .message = logString,
    };
    std::cout << GREEN_COLOR << logEntry.message << RESET_COLOR << std::endl;
    messages.push_back(logEntry);
}

void Logger::Err(const std::string& message) {
    std::string errString = std::format("ERR: [{}] - {}", std::chrono::system_clock::now(), message);
    LogEntry logEntry = {
        .type = LogType::LOG_ERROR,
        .message = errString,
    };
    std::cerr << RED_COLOR << logEntry.message << RESET_COLOR << std::endl;
    messages.push_back(logEntry);
}
