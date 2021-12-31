#include "thread_pool.hpp"

using namespace utility_space;

/*------------------------------------------------------------*/
/*-------------------------THREAD POOL------------------------*/
/*------------------------------------------------------------*/
template <typename R>
ThreadPool<R>::ThreadPool(size_t poolSize) :
    _sem(poolSize), _isStopCallback(false),
    _callbackThPtr(new std::thread(&ThreadPool::checkTasks, this))
{

}

template <typename R>
ThreadPool<R>::~ThreadPool()
{
    _isStopCallback.store(true);

    if(_callbackThPtr->joinable())
        _callbackThPtr->join();
}

template <typename R>
void ThreadPool<R>::addFunction(std::function<R(void)> func)
{
    _sem.acquire();

    putFunction(func);
}

template <typename R>
bool ThreadPool<R>::tryAddFunction(std::function<R(void)> func)
{
    if(_sem.tryAcquire() == false)
        return false;

    putFunction(func);
}

template <typename R>
R ThreadPool<R>::getAnyCalculatedResult()
{
    std::lock_guard<std::mutex> lock(_callbackMut);

    typename std::list<Task>::iterator it = std::find_if(_pool.begin(), _pool.end(),
        [](Task const &taskPair) -> bool
    {
        return taskPair.first == ThreadPool::FINISHED;
    });

    if(it == _pool.end())
        throw ThreadPoolException("Taskpool is empty.", ThreadPoolException::EMPTY_TASK_POOL);

    R res = it->second.get();

    _pool.erase(it);

    _sem.release();

    return res;
}

template <typename R>
R ThreadPool<R>::getFirstTaskResult()
{
    std::lock_guard<std::mutex> guard(_callbackMut);

    if(_pool.size() == 0)
        throw ThreadPoolException("Taskpool is empty.", ThreadPoolException::EMPTY_TASK_POOL);

    Task taskPair = _pool.pop_front();
    _sem.release();

    return taskPair.second.get();
}

template <typename R>
R ThreadPool<R>::getLastTaskResult()
{
    std::lock_guard<std::mutex> guard(_callbackMut);

    if(_pool.size() == 0)
        throw ThreadPoolException("Taskpool is empty.", ThreadPoolException::EMPTY_TASK_POOL);

    Task taskPair = _pool.pop_back();
    _sem.release();

    return taskPair.second.get();
}

template <typename R>
void ThreadPool<R>::flushAllCalculated()
{
    std::lock_guard<std::mutex> guard(_callbackMut);

    _pool.remove_if([](Task const &taskPair) -> bool
    {
        return taskPair.first == ThreadPool::FINISHED;
    });
}

template <typename R>
void ThreadPool<R>::flushAll()
{
    std::lock_guard<std::mutex> guard(_callbackMut);
    _pool.clear();
}

template <typename R>
void ThreadPool<R>::checkTasks()
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

template <typename R>
void ThreadPool<R>::putFunction(std::function<R(void)> func)
{
    std::future<R> res = std::async(std::launch::async, func);

    std::lock_guard<std::mutex> guard(_callbackMut);
    _pool.push_back({ThreadPool::STARTED, std::move(res)});
}
