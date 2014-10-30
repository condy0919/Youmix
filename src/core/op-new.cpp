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

// Broken. Please don't use it.
void *operator new(size_t, void *p) { return p; }
void *operator new [](size_t, void *p) { return p; }
void operator delete(void *, void *) {}
void operator delete [](void *, void *) {}

} // extern "C++"
