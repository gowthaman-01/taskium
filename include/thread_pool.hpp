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
    // Workers are stored as unique_ptrs because std::vector::push_back requires movable elements.
    std::vector<std::unique_ptr<Worker>> workers_;

public:
    /**
     * @brief Constructs a ThreadPool with a fixed number of worker threads.
     *
     * @param N The number of worker threads to create.
     */
    ThreadPool(size_t N);

    /**
     * @brief Submits a task to the thread pool. One worker thread will be notified to execute the task.
     *
     * @param task The task to execute, represented as a std::function<void()>.
     */
    void submit(std::function<void()> task);

    /**
     * @brief Gracefully shuts down the thread pool.
     *
     * Signals all worker threads to stop processing new tasks and exit their execution loops.
     * Wakes up any sleeping workers and waits for them to finish before cleaning up resources.
     */
    void shutdown();

};

} // namespace Taskium
