#pragma once

#include <atomic>
#include <functional>
#include <thread>

#include "safe_queue.hpp"

namespace Taskium {

class Worker {
private:
    std::thread t_;

public:
    explicit Worker(SafeQueue<std::function<void()>>& safe_queue, std::atomic<bool>& stop_flag);
    ~Worker() noexcept;
    
    Worker(const Worker& other) = delete;
    Worker& operator=(const Worker& other) = delete;
    
    Worker(Worker&& other) = delete;
    Worker& operator=(Worker&& other) = delete;
    
    void loop(SafeQueue<std::function<void()>>& safe_queue, std::atomic<bool>& stop_flag);
};

} // namespace Taskium
