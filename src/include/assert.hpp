#ifndef _YOUMIX_ASSERT_H_
#define _YOUMIX_ASSERT_H_

#include "ostream.hpp"
#include "elf.hpp"

#ifndef NDEBUG
#define assert(x) \
    do { \
        if (!(x)) { \
            kernel_panic(); \
            print_seg_status(); \
            io::cout << "Assertion Failed: " << #x << ": File: " << __FILE__ << ", Line: " << io::dec << __LINE__ << io::endl; \
            while (true); \
        } \
    } while (0)
#else
#define assert(x) (x)
#endif


void kernel_panic();
void print_seg_status();

#endif
