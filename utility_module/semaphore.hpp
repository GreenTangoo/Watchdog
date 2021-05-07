#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <mutex>
#include <condition_variable>

namespace utility_space
{
    class Semaphore
    {
    public:
        Semaphore(int count = 0);
        Semaphore(Semaphore const &other) = delete;
        Semaphore(Semaphore &&other) = delete;
        ~Semaphore();
        Semaphore const& operator=(Semaphore const &other) = delete;
        Semaphore const& operator=(Semaphore &&other) = delete;
        void acquire();
        bool tryAcquire();
        void release();
    private:
        std::mutex _mut;
        std::condition_variable _cond;
        int _count;
    };
}

#endif // SEMAPHORE_HPP