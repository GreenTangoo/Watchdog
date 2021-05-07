#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <future>
#include <list>
#include <mutex>
#include <functional>

#include "semaphore.hpp"

namespace utility_space
{

    template <typename R, typename ...Args> 
    class ThreadPool
    {
    public:
        explicit ThreadPool();
        explicit ThreadPool(size_t poolSize);
        ThreadPool(ThreadPool const &other) = delete;
        ThreadPool(ThreadPool &&other) = delete;
        ~ThreadPool();
        ThreadPool const& operator=(ThreadPool const &other) = delete;
        ThreadPool const& operator=(ThreadPool &&other) = delete;
        void addFunction(std::function<R(Args...)> func);
        void tryAddFunction(std::function<R(Args...)> func);
        std::future<R> getAnyCalculatedResult();
        std::future<R> getFirstTastResult();
        std::future<R> getLastTaskResult();
        void flushAllCalculated();
        void flushAll();
    public:
        enum taskStatus { NOT_STARTED = 0, STARTED, FINISHED };
    private:
        typedef std::pair<taskStatus, std::future<R>> Task;
        size_t _poolSize;
        Semaphore _sem;
        std::list<Task> _pool;
    };

    #include "thread_pool_impl.hpp"

}

#endif // THREAD_POOL_HPP