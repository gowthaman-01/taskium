#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace Taskium {

template <typename T>
class SafeQueue {
private:
    std::queue<T> q_;
    std::mutex m_;
    std::condition_variable cv_;
    
public:
    /**
     * @brief Adds a new element to the queue.
     *
     * If one or more threads are waiting in wait_and_pop(),
     * one of them will be notified.
     *
     * @param value The item to push into the queue (moved or copied).
     */
    void push(T value);
    
    /**
     * @brief Attempts to pop the front element without blocking.
     *
     * If the queue is empty, returns false immediately.
     * Else pops the front element and assigns it to the provided reference.
     *
     * @param value Reference where the popped element will be stored.
     * @return true if an element was popped, false if the queue was empty.
     */
    bool try_pop(T& value);
    
    /**
     * @brief Checks if the queue is empty in a thread-safe manner.
     *
     * Acquires the internal mutex before accessing the underlying queue.
     * Safe to call from any thread at any time.
     *
     * @return true if the queue is empty, false otherwise.
     */
    bool empty() const;

    /**
     * @brief Checks if the queue is empty without acquiring the mutex.
     *
     * Assumes the caller has already locked the queue’s mutex externally.
     * Use with caution: only call this when the mutex is already held,
     * such as inside a condition variable wait predicate.
     *
     * @return true if the queue is empty, false otherwise.
     */
    bool unsafe_empty() const;
    
    // Getters
    std::mutex& get_mutex();
    std::condition_variable& get_cv();
};

template <typename T>
void SafeQueue<T>::push(T value) {
    std::lock_guard<std::mutex> lock(m_);
    q_.push(value);
    cv_.notify_one();
}

template <typename T>
bool SafeQueue<T>::try_pop(T &value) {
    std::lock_guard<std::mutex> lock(m_);
    if (q_.empty()) {
        return false;
    }
    
    value = q_.front();
    q_.pop();
    return true;
}

template <typename T>
bool SafeQueue<T>::empty() const {
    std::lock_guard<std::mutex> lock(m_);
    return q_.empty();
}

template <typename T>
bool SafeQueue<T>::unsafe_empty() const {
    return q_.empty();
}


template <typename T>
std::mutex& SafeQueue<T>::get_mutex() {
    return m_;
}

template <typename T>
std::condition_variable& SafeQueue<T>::get_cv() {
    return cv_;
}

} // namespace Taskium
