#include "../include/thread.hpp"
#include "../include/task_struct.hpp"
#include "../include/klayout.hpp"
#include "../libs/list"

extern std::list<void*> task_queue, join_queue;

void thread_exit() {
    uint32_t bp;
    __asm__ __volatile__("mov %%ebp, %0" : "=r"(bp));
    bp &= ~(KERNEL_STACK_SIZE - 1);
    void *t = (void*)(*(uint32_t*)(bp + KERNEL_STACK_SIZE - sizeof(uint32_t)));

    task_queue.remove(t);
    join_queue.push_back(t);
    while (true)
        __asm__ __volatile__("pause" : : : "memory");
}

