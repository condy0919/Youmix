#ifndef _YOUMIX_KLAYOUT_H_
#define _YOUMIX_KLAYOUT_H_

#include <stdint.h>
#include <stddef.h>

#define V2P(_addr_) (uint32_t(_addr_) - KERNEL_VIRTUAL_BASE)
#define P2V(_addr_) (uint32_t(_addr_) + KERNEL_VIRTUAL_BASE)

#define PAGE_PRESENT 0x1
#define PAGE_WRITABLE 0x2
#define PAGE_MODE 0x4
#define PAGE_8MB_SIZE 0x8

const uint32_t KERNEL_VIRTUAL_BASE = 0xc0000000;
const uint32_t KERNEL_HEAP = 0xc8000000;

#endif
