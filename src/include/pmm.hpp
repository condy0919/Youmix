#ifndef _YOUMIX_PMM_H_
#define _YOUMIX_PMM_H_

#include <stdint.h>
#include <stddef.h>

#include "multiboot.h"
#include "../libs/util_list.h"

const uint32_t PAGE_SIZE = 4096;
const uint32_t RAM_MAXSIZE = 128 * 1024 * 1024;
const uint32_t RAM_MAXPAGE = RAM_MAXSIZE / PAGE_SIZE;
const uint32_t PAGE_MASK = 0xffffffff ^ (PAGE_SIZE - 1);

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
    zone_t();// = default;

    void init_zone();
    void *alloc(int);
    void dealloc(void *);

    uint32_t free_pages;
    free_area_t free_area[11];
    uint32_t max_free_pages;
    uint8_t (*mem_map)[PAGE_SIZE];

private:
    int find_order(int);
};

void memory_layout();

#endif
