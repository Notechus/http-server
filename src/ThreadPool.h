#ifndef HTTP_SERVER_THREADPOOL_H
#define HTTP_SERVER_THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

// https://www.geeksforgeeks.org/cpp/thread-pool-in-cpp/
class ThreadPool {
public:
    ThreadPool(size_t num_threads);

    ~ThreadPool();

    void enqueue(std::function<void()> task);

private:
    // Vector to store worker threads
    std::vector<std::thread> threads_;

    // Queue of tasks
    std::queue<std::function<void()> > tasks_;

    // Mutex to synchronize access to shared data
    std::mutex queue_mutex_;

    // Condition variable to signal changes in the state of
    // the tasks queue
    std::condition_variable cv_;

    // Flag to indicate whether the thread pool should stop
    // or not
    bool stop_ = false;
};


#endif //HTTP_SERVER_THREADPOOL_H
