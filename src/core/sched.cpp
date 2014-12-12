#include "../include/sched.hpp"

std::list<void*> task_queue, join_queue;

void init_sched() {
    new K::thread<void>;
}

void schedule() {
    static void *__prev = nullptr;
    if (!__prev) {
        __prev = task_queue.front();
    } else {
        // Swap environment
        auto *prev = reinterpret_cast<K::thread<void> *>(__prev),
             *cur = reinterpret_cast<K::thread<void> *>(task_queue.front());
        __prev = cur;
        task_queue.pop_front();
        task_queue.push_back(cur);
        if (cur != prev) {
            __asm__ __volatile__("mov %%ebx, %0" : "=a"(prev->task.cntx.ebx));
            __asm__ __volatile__("mov %%esi, %0" : "=a"(prev->task.cntx.esi));
            __asm__ __volatile__("mov %%edi, %0" : "=a"(prev->task.cntx.edi));
            __asm__ __volatile__("mov %%esp, %0" : "=a"(prev->task.cntx.esp));
            __asm__ __volatile__("mov %%ebp, %0" : "=a"(prev->task.cntx.ebp));
            __asm__ __volatile__("pushf");
            __asm__ __volatile__("pop %0" : "=a"(prev->task.cntx.eflags));

            __asm__ __volatile__("push %0" : : "a"(cur->task.cntx.eflags));
            __asm__ __volatile__("popf");
            __asm__ __volatile__("mov %0, %%ebx" : : "a"(cur->task.cntx.ebx));
            __asm__ __volatile__("mov %0, %%esi" : : "a"(cur->task.cntx.esi));
            __asm__ __volatile__("mov %0, %%edi" : : "a"(cur->task.cntx.edi));
            __asm__ __volatile__("mov %0, %%esp" : : "a"(cur->task.cntx.esp));
            __asm__ __volatile__("mov %0, %%ebp" : : "a"(cur->task.cntx.ebp));
        }
    }
}
