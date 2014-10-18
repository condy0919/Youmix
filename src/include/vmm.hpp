#pragma once

#include "pmm.hpp"
#include "idt.hpp"
#include "../libs/string.h"
#include "klayout.hpp"

#define PD_IDX(x) ((x) >> 22)
#define PTE_IDX(x) (((x) >> 12) & 0x3ff)
#define PI_IDX(x) ((x) & 0xfff)

#define PAGE_PRESENT 0x1
#define PAGE_WRITABLE 0x2

const uint32_t PAGE_TABLE_ENTRY_COUNT = RAM_MAXPAGE / 1024;

extern "C" void init_page();
void map(uint32_t *, uint32_t, uint32_t, uint32_t);
void unmap(uint32_t *, uint32_t);
uint32_t get_mapping(uint32_t *, uint32_t);
void page_fault(Register *);
