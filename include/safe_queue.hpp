#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace Taskium {

template <typename T>
class SafeQueue {
private:
    std::queue<T> queue_;
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
    [[nodiscard]] bool try_pop(T& value);
    
    /**
     * @brief Pops the front element without locking.
     *
     * This method assumes the caller already holds the mutex.
     * If the queue is empty, returns false immediately.
     * Else pops the front element and assigns it to the provided reference.
     *
     * @param value Reference where the popped element will be stored.
     * @return true if an element was popped, false if the queue was empty.
     */
    [[nodiscard]] bool unsafe_try_pop(T& value);
    
    /**
     * @brief Checks if the queue is empty in a thread-safe manner.
     *
     * Acquires the internal mutex before accessing the underlying queue.
     * Safe to call from any thread at any time.
     *
     * @return true if the queue is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const;

    /**
     * @brief Checks if the queue is empty without acquiring the mutex.
     *
     * Assumes the caller has already locked the queue’s mutex externally.
     * Use with caution: only call this when the mutex is already held,
     * such as inside a condition variable wait predicate.
     *
     * @return true if the queue is empty, false otherwise.
     */
    [[nodiscard]] bool unsafe_empty() const;
    
    // Getters
    [[nodiscard]] std::mutex& get_mutex();
    [[nodiscard]] std::condition_variable& get_cv();
};

template <typename T>
void SafeQueue<T>::push(T value) {
    std::lock_guard<std::mutex> lock(m_);
    queue_.push(value);
    cv_.notify_one();
}

template <typename T>
bool SafeQueue<T>::try_pop(T &value) {
    std::lock_guard<std::mutex> lock(m_);
    return unsafe_try_pop(value);
}

template <typename T>
bool SafeQueue<T>::unsafe_try_pop(T &value) {
    if (queue_.empty()) {
        return false;
    }
    
    value = queue_.front();
    queue_.pop();
    return true;
}

template <typename T>
bool SafeQueue<T>::empty() const {
    std::lock_guard<std::mutex> lock(m_);
    return queue_.empty();
}

template <typename T>
bool SafeQueue<T>::unsafe_empty() const {
    return queue_.empty();
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
