#include <stdint.h>
#include <stddef.h>
#include "../include/mm.hpp"

extern "C++" {

void *operator new(size_t size) {
    return Memory::heap.alloc(size);
}

void *operator new[](size_t size) {
    return Memory::heap.alloc(size);
}

void operator delete(void *p) {
    Memory::heap.dealloc(p);
}

void operator delete[](void *p) {
    Memory::heap.dealloc(p);
}

/*
 * §18.4.1.3 Placement forms
 * These functions are reserved, a C++ program may not define functions that displace the versions in the Standard C++ library.
 */
//void* operator new(size_t, void *p) { return p; }
//void* operator new[](size_t, void *p) { return p; }
//void operator delete(void*, void*) {}
//void operator delete[](void*, void*) {}

} // extern "C++"
