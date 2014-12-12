#ifndef _YOUMIX_KLAYOUT_H_
#define _YOUMIX_KLAYOUT_H_

#include <stdint.h>
#include <stddef.h>

#define V2P(_addr_) (uint32_t(_addr_) - KERNEL_VIRTUAL_BASE)
#define P2V(_addr_) (uint32_t(_addr_) + KERNEL_VIRTUAL_BASE)

const uint32_t PAGE_PRESENT = 0x1;
const uint32_t PAGE_WRITABLE = 0x2;
const uint32_t PAGE_MODE = 0x4;
const uint32_t PAGE_8MB_SIZE = 0x8;

const uint32_t PAGE_SIZE = 4096;
const uint32_t RAM_MAXSIZE = 128 * 1024 * 1024; // 128 MiB
const uint32_t RAM_MAXPAGE = RAM_MAXSIZE / PAGE_SIZE;
const uint32_t PAGE_MASK = 0xffffffff ^ (PAGE_SIZE - 1);
const uint32_t PAGE_TABLE_ENTRY_COUNT = RAM_MAXPAGE / 1024;

const uint32_t KERNEL_VIRTUAL_BASE = 0xc0000000;
//const uint32_t KERNEL_HEAP = KERNEL_VIRTUAL_BASE + RAM_MAXSIZE;

const uint32_t KERNEL_STACK_SIZE = 4096;

#endif
