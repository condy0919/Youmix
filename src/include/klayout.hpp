#ifndef _YOUMIX_KLAYOUT_H_
#define _YOUMIX_KLAYOUT_H_

#include <stdint.h>
#include <stddef.h>

#define V2P(_addr_) (uint32_t(_addr_) - KERNEL_VIRTUAL_BASE)
#define P2V(_addr_) (uint32_t(_addr_) + KERNEL_VIRTUAL_BASE)

const uint32_t KERNEL_VIRTUAL_BASE = 0xc0000000;
const uint32_t KERNEL_HEAP = 0xc8000000;

#endif
