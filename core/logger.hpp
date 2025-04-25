#pragma once

#include <mutex>
#include <string_view>

namespace Taskium {

class Logger {
private:
    std::mutex m_;
public:
    void general(std::string_view message);
    
    void info(std::string_view message);
    
    void error(std::string_view message);
};

} // namespace Taskium
