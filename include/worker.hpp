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
    
    /**
     * @brief The main execution loop for a worker thread.
     *
     * Continuously waits for tasks from the task queue and executes them.
     * The loop exits gracefully when the stop flag is set and the queue is empty.
     *
     * @param safe_queue The shared task queue from which tasks are retrieved.
     * @param stop_flag The atomic flag indicating whether the thread pool is shutting down.
     */
    void loop(SafeQueue<std::function<void()>>& safe_queue, std::atomic<bool>& stop_flag);
};

} // namespace Taskium
