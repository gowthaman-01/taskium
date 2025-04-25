#include "logger.hpp"

#include <iostream>
#include <mutex>
#include <string_view>

namespace Taskium {

void Logger::general(std::string_view message) {
    std::lock_guard<std::mutex> lock(m_);
    std::cout << message << '\n';
}

void Logger::info(std::string_view message) {
    std::lock_guard<std::mutex> lock(m_);
    std::cout << "[INFO] " << message << '\n';
}

void Logger::error(std::string_view message) {
    std::lock_guard<std::mutex> lock(m_);
    std::cerr << "[ERROR] " << message << '\n';
}

} // namespace Taskium
