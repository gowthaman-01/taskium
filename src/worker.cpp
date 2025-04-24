#include <mutex>

#include "../include/worker.hpp"

void Taskium::Worker::loop() {
    auto& m = safe_queue_.get_mutex();
    auto& cv = safe_queue_.get_cv();
    
    std::unique_lock<std::mutex> lock(m);
    
    while (true) {
        // Wait until there is work or shutdown signal
        cv.wait(lock, [this]() { return stop_flag_ || !safe_queue_.unsafe_empty(); });
        
        // Shutdown thread
        if (stop_flag_ && safe_queue_.unsafe_empty()) {
            break;
        }
        
        std::function<void()> task;
        bool task_acquired = safe_queue_.try_pop(task);
        if (!task_acquired) {
            continue;
        }

        lock.unlock();
        task();
        lock.lock();
    }
}
