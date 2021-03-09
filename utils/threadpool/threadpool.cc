#include "threadpool.h"

ThreadPool::ThreadPool(size_t _n_threads)
        : stop_(false) {
    while (_n_threads--) {
        __CreateWorkerThread();
    }
}

void ThreadPool::__CreateWorkerThread() {
    workers_.emplace_back([this] {
        while (true) {
            std::function<void()> task;
            TaskProfile *profile;
            std::list<std::pair<TaskProfile*, std::function<void()>>>::iterator idx;
            {
                ScopeLock lock(this->mutex_);
                while (true) {
                    this->cv_.wait(lock, [&, this] {
//                        return (idx = this->__SelectTask()) >= 0 || this->stop_;
                        return 0;
                    });
                    
                    if (idx != tasks_.end()) {
//                        profile = this->tasks_[idx].first;

                        uint64_t wait_time = __ComputeWaitTime(profile);

                        if (wait_time == 0) { break; }

                        size_t curr_size = tasks_.size();
                        bool is_new = this->cv_.wait_for(lock, std::chrono::milliseconds(wait_time),
                                [&, this] { return this->__IsHigherPriorityTaskAddWhenWaiting(profile, curr_size); });
                        if (is_new) {  // Comes a task that needs to be done earlier
                            continue;
                        }
                        break;
                    }
                    // no task needs to be executed or will be executed
                    return;
                }
    
//                this->running_serial_tags_.insert(profile->serial_tag);
//                task = this->tasks_[idx].second;
//                if (profile->type == TaskProfile::kPeriodic) {
//                    profile->record = ::gettickcount();
//                    profile->seq = TaskProfile::__MakeSeq();
//                } else {
//                    tasks_.erase(idx);
//                }
            }
            task();
            {
                ScopeLock lock(this->mutex_);
                this->running_serial_tags_.erase(profile->serial_tag);
                if (profile->type != TaskProfile::kPeriodic) { delete profile; }
            }
        }
    });
}


std::list<std::pair<TaskProfile*, std::function<void()>>>::iterator
ThreadPool::__SelectTask() {
    static uint64_t last_selected_seq = TaskProfile::kInvalidSeq;
    
    uint64_t now = ::gettickcount();
    uint64_t min_wait_time = 0xffffffffffffffff;
    ssize_t min_wait_time_task_idx = -1;
    return tasks_.begin();
}

bool ThreadPool::__IsHigherPriorityTaskAddWhenWaiting(TaskProfile *_lhs, size_t _old_n_tasks) {
    // FIXME
    if (tasks_.size() == _old_n_tasks) {
        return false;
    }
    TaskProfile *rhs = tasks_.back().first;
    if (rhs->type == TaskProfile::kImmediate) {
        return true;
    }
    uint64_t now = ::gettickcount();
    return __ComputeWaitTime(rhs, now) < __ComputeWaitTime(_lhs, now);
}

uint64_t ThreadPool::__ComputeWaitTime(TaskProfile *_profile, uint64_t _now) {
    if (!_profile) { return -1; }
    
    int64_t ret = 0;
    _now = _now == 0 ? ::gettickcount() : _now;
    
    if (_profile->type == TaskProfile::kAfter) {
        ret = _profile->record + _profile->after - _now;
    } else if (_profile->type == TaskProfile::kPeriodic) {
        ret = _profile->record + _profile->period - _now;
    }
    return ret > 0 ? ret : 0;
}

ThreadPool::~ThreadPool() {
    {
        ScopeLock lock(mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    for (std::thread &thread : workers_) {
        thread.join();
    }
    {
        ScopeLock lock(mutex_);
        for (auto & task : tasks_) {
            TaskProfile *profile = task.first;
            if (profile) {
                delete profile;
                task.first = NULL;
            }
        }
    }
}
