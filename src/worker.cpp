#include "../include/worker.hpp"

#include "../include/safe_queue.hpp"
#include "../include/thread_pool.hpp"

#include <atomic>
#include <functional>
#include <mutex>

using Taskium::Worker;

Worker::Worker(SafeQueue<std::function<void()>>& safe_queue, std::atomic<bool>& stop_flag)
    : t_(&Worker::loop, this, std::ref(safe_queue), std::ref(stop_flag)) {}

Worker::~Worker() noexcept {
    if (t_.joinable()) {
        t_.join();
    }
}

void Worker::loop(SafeQueue<std::function<void()>>& safe_queue, std::atomic<bool>& stop_flag) {
    auto& m = safe_queue.get_mutex();
    auto& cv = safe_queue.get_cv();
    
    while (true) {
        std::unique_lock<std::mutex> lock(m);
        
        // Wait until there is work or shutdown signal
        cv.wait(lock, [&stop_flag, &safe_queue]() { return stop_flag || !safe_queue.unsafe_empty(); });
        
        // Shutdown thread
        if (stop_flag && safe_queue.unsafe_empty()) {
            break;
        }
        
        std::function<void()> task;
        bool task_acquired = safe_queue.unsafe_try_pop(task);
        if (!task_acquired) {
            continue;
        }

        lock.unlock();
        task();
        lock.lock();
    }
}
