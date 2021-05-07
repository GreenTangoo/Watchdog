#include "semaphore.hpp"

using namespace utility_space;

Semaphore::Semaphore(int count) : _count(count)
{

}

Semaphore::~Semaphore()
{

}

void Semaphore::acquire()
{
    std::unique_lock<std::mutex> lock(_mut);
    if(_count == 0)
        _cond.wait(lock);

    _count--;
}

bool Semaphore::tryAcquire()
{
    std::lock_guard<std::mutex> guard(_mut);
    if(_count == 0)
        return false;
    else
    {
        _count--;
        return true;
    }
}

void Semaphore::release()
{
    std::lock_guard<std::mutex> guard(_mut);
    _count++;
    _cond.notify_one();
}