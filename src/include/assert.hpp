/*
 * ISO C99 Standard: 7.2 Diagnostics   <assert.h>
 */
#ifdef _YOUMIX_ASSERT_H_

#undef _YOUMIX_ASSERT_H_
#undef assert

#endif // assert.h

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
#define assert(x) (void)0
#endif

void kernel_panic();
void print_seg_status();
