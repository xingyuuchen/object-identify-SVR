#ifndef OI_SVR_THREADPOOL_H
#define OI_SVR_THREADPOOL_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <queue>

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
    std::future<typename std::result_of<F(Args...)>::type>
    Execute(F&& _f, Args&&... _args) {
        using return_t = typename std::result_of<F(Args...)>::type;
        using pack_task_t = std::packaged_task<return_t(void)>;
        
        auto task = std::make_shared<pack_task_t>(std::bind(_f, _args...));
        std::future<return_t> ret = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.emplace([=] () -> void { (*task)(); });
        }
        cv_.notify_one();
        return ret;
    }

  private:
    ThreadPool(size_t _n_threads = 4);

  private:
    std::vector<std::thread>                    worker_threads_;
    std::queue<std::function<void()>>           tasks_;
    std::mutex                                  mutex_;
    std::condition_variable                     cv_;
    bool                                        working_;
    
};

#endif //OI_SVR_THREADPOOL_H
