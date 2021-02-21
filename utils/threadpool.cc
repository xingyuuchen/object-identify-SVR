#include "threadpool.h"

ThreadPool::ThreadPool(size_t _n_threads)
        : working_(true) {
    for (size_t i = 0; i < _n_threads; i++) {
        worker_threads_.emplace_back(
                [this] () -> void {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    cv_.wait(lock, [this] () -> bool {
                        return !this->tasks_.empty() || !this->working_;
                    });
                    if (!working_ && tasks_.empty()) {
                        return;
                    }
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        working_ = false;
    }
    cv_.notify_all();
    for (std::thread &thread : worker_threads_) {
        thread.join();
    }
}
