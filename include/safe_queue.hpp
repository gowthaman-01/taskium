#include <condition_variable>
#include <mutex>
#include <queue>

namespace gstd {

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
     * @brief Waits until the queue is non-empty and pops the front element.
     *
     * This function blocks the calling thread if the queue is empty,
     * waking up when an item becomes available. The popped element
     * is assigned to the provided reference.
     *
     * @param value Reference where the popped element will be stored.
     */
    void wait_and_pop(T& value);
    
    /**
     * @brief Attempts to pop the front element without blocking.
     *
     * If the queue is empty, returns false immediately without blocking.
     * Else pops the front element and assigns it to the provided reference.
     *
     * @param value Reference where the popped element will be stored.
     * @return true if an element was popped, false if the queue was empty.
     */
    bool try_pop(T& value);
    
    bool empty() const;
};

template <typename T>
void SafeQueue<T>::push(T value) {
    std::lock_guard<std::mutex> lock(m_);
    q_.push(value);
    cv_.notify_one();
}

template <typename T>
void SafeQueue<T>::wait_and_pop(T& value) {
    std::unique_lock<std::mutex> lock(m_);
    cv_.wait(lock, [this]() { return !q_.empty(); });
    
    value = q_.front();
    q_.pop();
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

} // namespace gstd
