#ifndef _YOUMIX_KTHREAD_HPP_
#define _YOUMIX_KTHREAD_HPP_

#include <stdint.h>
#include <stddef.h>

#include "../libs/utility"
#include "../libs/list"
#include "../libs/algorithm"

#include "klayout.hpp"
#include "task_struct.hpp"

extern "C" void thread_exit() __attribute__((noinline));
extern std::list<void*> task_queue, join_queue;

namespace K {
template<typename Callable>
class thread {
public:
    template<typename... Args>
    thread(Callable*, Args&&...);
    thread();
    ~thread();

    int get_id() const;
    bool joinable() const;
    void join();

    task_struct task;
    Callable *fn;

private:
    void push_args(uint32_t*&);

    template<typename T, typename... Args>
    void push_args(uint32_t*&, T&&, Args&&...);
};


template<typename Callable>
template<typename... Args>
thread<Callable>::thread(Callable *fn, Args&&... args) : task(), fn(fn) {
    task.stack = new char[KERNEL_STACK_SIZE];
    uint32_t *sp = reinterpret_cast<uint32_t*>(task.stack + KERNEL_STACK_SIZE);
    
    *--sp = reinterpret_cast<uint32_t>(this); // For thread exit.
    push_args(sp, args...);
    *--sp = reinterpret_cast<uint32_t>(thread_exit);
    *--sp = reinterpret_cast<uint32_t>(fn);
    *--sp = 0; // Stub. It depends your schedule function. In this occasion,
               // context switch happens in schedule function(noinline) which
               // has `pop ebp' instruction. This Stub makes sense.
    task.cntx.ebp = task.cntx.esp = reinterpret_cast<uint32_t>(sp);
    task.cntx.eflags = 0x200;

    task_queue.push_back(this);
}

template<typename Callable>
thread<Callable>::thread() : task(), fn(0) {
    task_queue.push_back(this);
}

template<typename Callable>
thread<Callable>::~thread() {
    if (task.stack)
        delete[] task.stack;
    join_queue.remove(this);
}

template<typename Callable>
int thread<Callable>::get_id() const { return task.id; }

template<typename Callable>
bool thread<Callable>::joinable() const {
    auto iter = std::find(join_queue.begin(), join_queue.end(), this);
    return iter != join_queue.end();
}

template<typename Callable>
void thread<Callable>::join() {
    while (std::find(join_queue.begin(), join_queue.end(), this) ==
           join_queue.end())
        __asm__ __volatile__("pause" : : : "memory");
    join_queue.remove(this);
}

template<typename Callable>
template<typename T, typename... Args>
void thread<Callable>::push_args(uint32_t*& sp, T&& head, Args&&... rest) {
    push_args(sp, rest...);
    *--sp = reinterpret_cast<uint32_t&>(head);
}

template<typename Callable>
void thread<Callable>::push_args(uint32_t*&) {}

} // namespace of K

#endif
