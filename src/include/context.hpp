#ifndef _YOUMIX_CONTEXT_HPP_
#define _YOUMIX_CONTEXT_HPP_

#include <stdint.h>
#include <stddef.h>

struct TaskContext {
    uint32_t ebx, esp, ebp, esi, edi;
    uint32_t eflags;
};

#endif
