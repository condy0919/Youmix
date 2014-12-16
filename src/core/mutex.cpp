#include "../include/mutex.hpp"
#include "../include/log.hpp"

#ifdef __cplusplus
extern "C" {
#endif
unsigned kthread_mutex_trylock(std::mutex::kthread_mutex_t*);
void kthread_mutex_lock(std::mutex::kthread_mutex_t*);
void kthread_mutex_unlock(std::mutex::kthread_mutex_t*);
#ifdef __cplusplus
}
#endif

namespace std {
void mutex::lock() {
#ifndef NDEBUG
    LOG("lock mutex");
#endif
    kthread_mutex_lock(&mtx);
#ifndef NDEBUG
    LOG("lock mutex done");
#endif
}

bool mutex::try_lock() { return kthread_mutex_trylock(&mtx); }

void mutex::unlock() {
#ifndef NDEBUG
    LOG("unlock mutex");
#endif
    kthread_mutex_unlock(&mtx);
#ifndef NDEBUG
    LOG("unlock mutex done");
#endif
}
}
