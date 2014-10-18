#include "../include/pmm.hpp"
#include "../include/ostream.hpp"

zone_t zone;

struct list_head *free_area_t::insert(struct list_head *p) {
    list_add(p, &free_list);
    ++nr_free;
    return p;
}

struct list_head *free_area_t::remove(struct list_head *p) {
    list_head *ret = p->prev;
    list_del(p);
    --nr_free;
    return ret;
}

void free_area_t::pop_front() {
    remove(free_list.next);
}

struct page *free_area_t::front() {
    struct page *p = list_entry(free_list.next, struct page, list);
    return p;
}

zone_t::zone_t() : free_pages(0), max_free_pages(0) {
    extern multiboot_info_t *glb_mboot_ptr;
    uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
    uint32_t mmap_len = glb_mboot_ptr->mmap_length;

    auto *beg = (multiboot_mmap_entry *)mmap_addr,
         *end = beg + mmap_len / sizeof(*beg);
    for (auto iter = beg; iter != end; ++iter) {
        if (iter->type == MULTIBOOT_MEMORY_AVAILABLE &&
            (iter->addr & 0x00000000FFFFFFFFUL) == 0x100000) {
            // skip the first 3M physical memory
            uint32_t mm_beg = (iter->addr & 0x00000000FFFFFFFFUL) + 0x300000,
                     mm_end = (iter->addr & 0x00000000FFFFFFFFUL) + iter->len;
            
            // initialize mem_map
            for (uint32_t mp = mm_beg; mp < mm_end && free_pages < RAM_MAXPAGE;
                 ++free_pages, mp += PAGE_SIZE) {
                mem_map[free_pages].p_addr = mp;
            }
            max_free_pages = free_pages;

            // fill free_area with trunk pages
            for (int order = 10, cur = 0; order >= 0; --order) {
                while (cur + (1UL << order) - 1 < free_pages) {
                    mem_map[cur].order = order;
                    free_area[order].insert(&mem_map[cur].list);
                    cur += (1UL << order);
                }
            }

            break;
        }
    }
}

struct page *zone_t::allocate(int order) {
    int cur_order;
    for (cur_order = order; cur_order <= 10; ++cur_order) {
        if (!free_area[cur_order].empty())
            break;
    }
    if (cur_order > 10)
        return nullptr;

    free_pages -= (1UL << order);
    struct page *p = free_area[cur_order].front();
    free_area[cur_order].pop_front();
    p->order = 0;
    uint32_t size = (1UL << cur_order);
    while (cur_order > order) {
        --cur_order;
        free_area[cur_order].insert(&p->list);
        p->order = cur_order;
        p += (size >>= 1);
    }
    return p;
}

void zone_t::deallocate(struct page *p, int order) {
    auto is_buddy = [&](struct page *p, int order) {
        auto page_reserved =
            [](struct page *p) { return p->list.next != &p->list; };
        return static_cast<uint32_t>(p - mem_map) < max_free_pages &&
               p->order == order && p->count == 0 && page_reserved(p);
    };

    if (!p)
        return;

    free_pages += (1UL << order);

    uint32_t page_idx = p - mem_map, buddy_idx = 0;
    p->reset(0);
    while (order < 10) {
        buddy_idx = page_idx ^ (1UL << order);
        struct page *buddy = mem_map + buddy_idx;
        if (!is_buddy(buddy, order))
            break;
        free_area[order].remove(&buddy->list);
        buddy->order = 0;
        page_idx &= buddy_idx;
        p = mem_map + page_idx;
        ++order;
    }
    free_area[order].insert(&p->list);
    p->reset(order);
}

void memory_layout() {
    extern multiboot_info_t *glb_mboot_ptr;

    uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
    uint32_t mmap_len = glb_mboot_ptr->mmap_length;

    auto *beg = (multiboot_mmap_entry *)mmap_addr,
         *end = beg + mmap_len / sizeof(*beg);

    // io::cout << "mmap_len = " << io::dec << mmap_len << io::endl;
    for (auto iter = beg; iter != end; ++iter)
        io::cout << "base_addr = " << io::hex << io::setw(16) << iter->addr
                 << io::setw(9) << " length = " << iter->len
                 << " type = " << io::setw(0) << io::dec << iter->type
                 << io::endl;
}

