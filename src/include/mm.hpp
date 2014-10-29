#ifndef _YOUMIX_MM_H_
#define _YOUMIX_MM_H_

#include <stdint.h>
#include <stddef.h>

#include "multiboot.h"
#include "idt.hpp"
#include "klayout.hpp"
#include "../libs/util_list.h"
#include "../libs/string.h"


namespace Memory {

inline uint32_t PD_IDX(uint32_t addr) { return addr >> 22; }

inline uint32_t PTE_IDX(uint32_t addr) { return (addr >> 12) & 0x3ff; }

inline uint32_t PI_IDX(uint32_t addr) { return addr & 0xfff; }

const uint32_t PAGE_SIZE = 4096;
const uint32_t RAM_MAXSIZE = 128 * 1024 * 1024; // 128 MiB
const uint32_t RAM_MAXPAGE = RAM_MAXSIZE / PAGE_SIZE;
const uint32_t PAGE_MASK = 0xffffffff ^ (PAGE_SIZE - 1);
const uint32_t PAGE_TABLE_ENTRY_COUNT = RAM_MAXPAGE / 1024;



struct page {
    page() : order(0), list(LIST_HEAD_INIT(list)) {}
    int order;
    struct list_head list;
};

struct free_area_t {
    free_area_t() : nr_free(0), free_list(LIST_HEAD_INIT(free_list)) {}

    struct list_head *insert(struct list_head *);
    struct list_head *remove(struct list_head *);

    void pop_front();
    struct page *front();

    bool empty() const { return nr_free == 0; }

    uint32_t nr_free;
    struct list_head free_list;
};

struct zone_t {
    zone_t();

    void init_zone();
    void *alloc(int);
    void dealloc(void *);

    uint32_t free_pages;
    free_area_t free_area[11];
    uint32_t max_free_pages;
    uint8_t (*mem_map)[PAGE_SIZE];

    int find_order(int);

    // Tricks to access page order
    int get_order(void *);
    void set_order(void *, uint8_t);

    // show status of memory
    static void memory_layout();
};



extern "C" void init_page_dir();
void map(uint32_t *, uint32_t, uint32_t, uint32_t);
void unmap(uint32_t *, uint32_t);
uint32_t get_mapping(uint32_t *, uint32_t);
void page_fault(IDT::Register *);

extern zone_t zone;

} // namespace Memory


#endif
