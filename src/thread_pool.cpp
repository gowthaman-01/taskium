#include "../include/thread_pool.hpp"

#include "../include/safe_queue.hpp"
#include "../include/worker.hpp"

#include <functional>
#include <memory>

using Taskium::ThreadPool;
using Taskium::Worker;

ThreadPool::ThreadPool(size_t N) : safe_queue_(), stop_flag_(false) {
    workers_.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        workers_.push_back(std::make_unique<Worker>(safe_queue_, stop_flag_));
    }
}

void ThreadPool::submit(std::function<void ()> task) {
    safe_queue_.push(task);
    safe_queue_.get_cv().notify_one();
}

void ThreadPool::shutdown() {
    stop_flag_ = true;
    safe_queue_.get_cv().notify_all();
}
