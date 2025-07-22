#include "logger.hpp"

#include <chrono>
#include <iostream>
#include <print>

constexpr auto GREEN_COLOR = "\e[0;32m";
constexpr auto RESET_COLOR = "\e[0m";
constexpr auto RED_COLOR = "\e[0;31m";

void Logger::Log(const std::string& message) {
    // LOG: [DD/MM/YYYY HH:MM:SS] - message, in green
    std::println("{}LOG: [{}] - {}{}", GREEN_COLOR, std::chrono::system_clock::now(), message,
                 RESET_COLOR);
}

void Logger::Err(const std::string& message) {
    // ERR : [ DATE ] - message, in red
    std::string errString = std::format("{}ERR: [{}] - {}{}", RED_COLOR,
                                        std::chrono::system_clock::now(), message, RESET_COLOR);
    std::cerr << errString << std::endl;
}
