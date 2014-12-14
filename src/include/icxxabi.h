/*
 * See details in
 * http://wiki.osdev.org/C%2B%2B#Local_Static_Variables_.28GCC_Only.29
 */
#ifndef _ICXXABI_H
#define _ICXXABI_H

#define ATEXIT_MAX_FUNCS 128

#ifdef __cplusplus
extern "C" {
#endif

struct atexit_func_entry_t {
    /*
    * Each member is at least 4 bytes large. Such that each entry is 12bytes.
    * 128 * 12 = 1.5KB exact.
    **/
    void (*destructor_func)(void *);
    void *obj_ptr;
    void *dso_handle;
};

int __cxa_atexit(void (*f)(void *), void *objptr, void *dso);
void __cxa_finalize(void *f);
void __cxa_pure_virtual();

#ifdef __cplusplus
};
#endif

namespace __cxxabiv1 {
    // Guard variables
    // The ABI requires a 64-bit type.
    __extension__ typedef int __guard __attribute__((mode(__DI__)));
    extern "C" {
        int __cxa_guard_acquire(__guard*);
        void __cxa_guard_release(__guard*);
        void __cxa_guard_abort(__guard*);
    }
}

#endif
