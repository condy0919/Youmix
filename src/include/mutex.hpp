#ifndef _YOUMIX_MUTEX_HPP_
#define _YOUMIX_MUTEX_HPP_

namespace std {
class mutex {
public:
    typedef unsigned kthread_mutex_t;

    constexpr mutex() : mtx(0) {}

    mutex(const mutex&) = delete;
    mutex(mutex&&) = delete;
    mutex& operator=(const mutex&) = delete;
    mutex& operator=(mutex&&) = delete;

    void lock();
    bool try_lock();
    void unlock();

private:
    kthread_mutex_t mtx;
};
}

#endif
