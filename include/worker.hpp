#pragma once

#include <atomic>
#include <functional>
#include <thread>

#include "safe_queue.hpp"

namespace Taskium {

class Worker {
private:
    SafeQueue<std::function<void()>>& safe_queue_;
    std::atomic<bool>& stop_flag_;
    std::thread t_;

public:
    Worker(SafeQueue<std::function<void()>>& safe_queue, std::atomic<bool>& stop_flag)
        : safe_queue_(safe_queue),
          stop_flag_(stop_flag),
          t_(&Worker::loop, this)  {}
    
    ~Worker() {
        if (t_.joinable()) {
            t_.join();
        }
    }
    
    Worker(const Worker& other) = delete;
    Worker& operator=(const Worker& other) = delete;
    Worker(Worker&& other) = delete;
    Worker& operator=(Worker&& other) = delete;
    
    void loop();
};

} // namespace Taskium
