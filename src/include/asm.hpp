#ifndef _YOUMIX_ASM_H_
#define _YOUMIX_ASM_H_

#include <stdint.h>
#include <stddef.h>


/*
 * Some ASM functions utils
 */

static inline void outb(uint16_t port, uint8_t value) {
     __asm__ __volatile__("outb %1, %0" : : "dN"(port), "a"(value));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

static inline void outw(uint16_t port, uint16_t value) {
    __asm__ __volatile__("outw %1, %0" : : "dN"(port), "a"(value));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ __volatile__("inw %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}


#endif
