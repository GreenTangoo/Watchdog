#include "thread_pool.hpp"

/*------------------------------------------------------------*/
/*-------------------------THREAD POOL------------------------*/
/*------------------------------------------------------------*/
template <typename R, typename ...Args> 
ThreadPool<R, Args...>::ThreadPool(size_t poolSize) : 
    _sem(poolSize), _isStopCallback(false), 
    _callbackThPtr(new std::thread(&ThreadPool::checkTasks, this))
{

}

template <typename R, typename ...Args> 
ThreadPool<R, Args...>::~ThreadPool()
{
    _isStopCallback.store(true);

    if(_callbackThPtr->joinable())
        _callbackThPtr->join();
}

template <typename R, typename ...Args> 
void ThreadPool<R, Args...>::addFunction(std::function<R(Args...)> func)
{
    _sem.acquire();
    
    putFunction(func);
}

template <typename R, typename ...Args> 
bool ThreadPool<R, Args...>::tryAddFunction(std::function<R(Args...)> func)
{
    if(_sem.tryAcquire() == false)
        return false;
    
    putFunction(func);
}

template <typename R, typename ...Args> 
R ThreadPool<R, Args...>::getAnyCalculatedResult()
{
    typename std::list<Task>::iterator it = std::find_if(_pool.begin(), _pool.end(),
        [](Task const &taskPair) -> bool
    {
        return taskPair.first == ThreadPool::FINISHED;
    });

    it->second.get();
}

template <typename R, typename ...Args> 
R ThreadPool<R, Args...>::getFirstTaskResult()
{
    Task taskPair = _pool.pop_front();
    return taskPair.second.get();
}

template <typename R, typename ...Args> 
R ThreadPool<R, Args...>::getLastTaskResult()
{
    Task taskPair = _pool.pop_back();
    return taskPair.second.get();
}

template <typename R, typename ...Args> 
void ThreadPool<R, Args...>::flushAllCalculated()
{
    _pool.remove_if([](Task const &taskPair) -> bool
    {
        return taskPair.first == ThreadPool::FINISHED;
    });
}

template <typename R, typename ...Args> 
void ThreadPool<R, Args...>::flushAll()
{
    std::lock_guard<std::mutex> guard(_callbackMut);
    _pool.clear();
}

template <typename R, typename ...Args> 
void ThreadPool<R, Args...>::checkTasks()
{
    while(!_isStopCallback.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::lock_guard<std::mutex> guard(_callbackMut);

        for(Task &oneTask : _pool)
        {
            std::future<R> &res = oneTask.second;
            if(res.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
                oneTask.first = ThreadPool::FINISHED;
        }
    }
}

template <typename R, typename ...Args> 
void ThreadPool<R,Args...>::putFunction(std::function<R(Args...)> func)
{
    std::future<R> res = std::async(std::launch::async, func);

    std::lock_guard<std::mutex> guard(_callbackMut);
    _pool.push_back({ThreadPool::STARTED, std::move(res)});
}