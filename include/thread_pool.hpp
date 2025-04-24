#pragma once

#include "safe_queue.hpp"
#include "worker.hpp"

#include <atomic>
#include <functional>
#include <memory>
#include <vector>

namespace Taskium {

class ThreadPool {
private:
    SafeQueue<std::function<void()>> safe_queue_;
    std::atomic<bool> stop_flag_;
    std::vector<std::unique_ptr<Worker>> workers_;

public:
    ThreadPool(size_t N);
    
    void submit(std::function<void()> task);
    
    void shutdown();
};

} // namespace Taskium
