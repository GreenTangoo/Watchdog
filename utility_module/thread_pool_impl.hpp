#include "thread_pool.hpp"

template <typename R, typename ...Args> 
ThreadPool<R, Args...>::ThreadPool()
{

}

template <typename R, typename ...Args> 
ThreadPool<R, Args...>::ThreadPool(size_t poolSize)
{

}

template <typename R, typename ...Args> 
ThreadPool<R, Args...>::~ThreadPool()
{

}

template <typename R, typename ...Args> 
void ThreadPool<R, Args...>::addFunction(std::function<R(Args...)> func)
{
}

template <typename R, typename ...Args> 
void ThreadPool<R, Args...>::tryAddFunction(std::function<R(Args...)> func)
{
    
}

template <typename R, typename ...Args> 
std::future<R> ThreadPool<R, Args...>::getAnyCalculatedResult()
{
}

template <typename R, typename ...Args> 
std::future<R> ThreadPool<R, Args...>::getFirstTastResult()
{

}

template <typename R, typename ...Args> 
std::future<R> ThreadPool<R, Args...>::getLastTaskResult()
{

}

template <typename R, typename ...Args> 
void ThreadPool<R, Args...>::flushAllCalculated()
{

}

template <typename R, typename ...Args> 
void ThreadPool<R, Args...>::flushAll()
{

}