#ifndef OI_SVR_THREADPOOL_H
#define OI_SVR_THREADPOOL_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <iostream>
#define cxy

#ifdef cxy

class ThreadPool {
  public:
    void operator=(ThreadPool const &) = delete;
    ThreadPool(ThreadPool const &) = delete;
    ~ThreadPool();
    
    static ThreadPool &Instance() {
        static ThreadPool instance;
        return instance;
    }
    
    
    template<class F, class... Args>
    void
    Execute(F&& _f, Args&&... _args) {
        using return_t = typename std::result_of<F(Args...)>::type;
        using pack_task_t = std::packaged_task<return_t(void)>;
        
        std::shared_ptr<pack_task_t> task = std::make_shared<pack_task_t>(std::bind(_f, _args...));
        
        {
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.emplace_back([=] () -> void {
                (*task)();
            });
        }
        cv_.notify_one();
    }

  private:
    ThreadPool(size_t _n_threads = 4);

  private:
    std::vector<std::thread>                    worker_threads_;
    std::vector<std::function<void()>>          tasks_;
    std::mutex                                  mutex_;
    std::condition_variable                     cv_;
    bool                                        working_;
    
};

#else

//#include <mutex>
//#include <condition_variable>
//#include <functional>
//#include <queue>
//#include <thread>
//
//class fixed_thread_pool {
//public:
//    explicit fixed_thread_pool(size_t thread_count)
//            : data_(std::make_shared<data>()) {
//        for (size_t i = 0; i < thread_count; ++i) {
//            std::thread([data = data_] {
//                std::unique_lock<std::mutex> lk(data->mtx_);
//                for (;;) {
//                    if (!data->tasks_.empty()) {
//                        auto current = std::move(data->tasks_.front());
//                        data->tasks_.pop();
//                        lk.unlock();
//                        current();
//                        lk.lock();
//                    } else if (data->is_shutdown_) {
//                        break;
//                    } else {
//                        data->cond_.wait(lk);
//                    }
//                }
//            }).detach();
//        }
//    }
//
//    fixed_thread_pool() = default;
//    fixed_thread_pool(fixed_thread_pool&&) = default;
//
//    ~fixed_thread_pool() {
//        if ((bool) data_) {
//            {
//                std::lock_guard<std::mutex> lk(data_->mtx_);
//                data_->is_shutdown_ = true;
//            }
//            data_->cond_.notify_all();
//        }
//    }
//
//    template <class F>
//    void execute(F&& task) {
//        {
//            std::lock_guard<std::mutex> lk(data_->mtx_);
//            data_->tasks_.emplace(std::forward<F>(task));
//        }
//        data_->cond_.notify_one();
//    }
//
//private:
//    struct data {
//        std::mutex mtx_;
//        std::condition_variable cond_;
//        bool is_shutdown_ = false;
//        std::queue<std::function<void()>> tasks_;
//    };
//    std::shared_ptr<data> data_;
//};



#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <iostream>

class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();
private:
    // need to keep track of threads so we can join them
    std::vector <std::thread> workers;
    // the task queue
    std::queue<std::function<void()>> tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
        : stop(false) {
    
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back(
                [this] {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);
                            this->condition.wait(lock,
                                                 [this] { return this->stop || !this->tasks.empty(); });
                            if (this->stop && this->tasks.empty())
                                return;
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }
                        task();
                    }
                }
        );
    }
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type(void)>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        
        // don't allow enqueueing after stopping the pool
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        
        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker: workers)
        worker.join();
}

#endif //cxy
#endif //OI_SVR_THREADPOOL_H
