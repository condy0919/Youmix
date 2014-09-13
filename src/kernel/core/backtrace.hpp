#pragma once

#include "ostream.hpp"
#include "elf.hpp"

#define assert(x) \
    do {\
        if (!(x))\
            kernel_panic();\
    } while (0)

void kernel_panic();

void print_seg_status();
