#ifndef _YOUMIX_MUTEX_HPP_
#define _YOUMIX_MUTEX_HPP_

namespace std {
class mutex {
public:
    typedef unsigned kthread_mutex_t;
    mutex();
    void lock();
    bool try_lock();
    void unlock();

private:
    kthread_mutex_t mtx;
};
}

#endif
