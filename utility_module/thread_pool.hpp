#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <future>
#include <list>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <functional>
#include <algorithm>

#include "semaphore.hpp"
#include "../exception_module/exceptions.hpp"

using namespace siem_ex_space;

namespace utility_space
{

    template <typename R>
    class ThreadPool
    {
    public:
        explicit ThreadPool(size_t poolSize);
        ThreadPool(ThreadPool const &other) = delete;
        ThreadPool(ThreadPool &&other) = delete;
        ~ThreadPool();
        ThreadPool const& operator=(ThreadPool const &other) = delete;
        ThreadPool const& operator=(ThreadPool &&other) = delete;
        void addFunction(std::function<R(void)> func);
        bool tryAddFunction(std::function<R(void)> func);
        R getAnyCalculatedResult();
        R getFirstTaskResult();
        R getLastTaskResult();
        void flushAllCalculated();
        void flushAll();
    private:
        void checkTasks();
        void putFunction(std::function<R(void)> func);
    public:
        enum taskStatus { NOT_STARTED = 0, STARTED, FINISHED };
    private:
        typedef std::pair<taskStatus, std::future<R>> Task;
        Semaphore _sem;
        std::atomic<bool> _isStopCallback;
        std::shared_ptr<std::thread> _callbackThPtr;
        std::mutex _callbackMut;
        std::list<Task> _pool;
    };

    #include "thread_pool_impl.hpp"

}

#endif // THREAD_POOL_HPP
