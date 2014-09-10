#pragma once

#include "console.hpp"
#include "elf.hpp"

#define assert(cond) \
    do {\
        if (!(cond))\
            kernel_panic();\
    } while (0)

void kernel_panic();

void print_seg_status();

