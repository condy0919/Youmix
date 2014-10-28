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

zone_t::zone_t() {
    extern multiboot_info_t *glb_mboot_ptr;
    uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
    uint32_t mmap_len = glb_mboot_ptr->mmap_length;

    auto *beg = (multiboot_mmap_entry *)mmap_addr,
         *end = beg + mmap_len / sizeof(*beg);
    for (auto iter = beg; iter != end; ++iter) {
        if (iter->type == MULTIBOOT_MEMORY_AVAILABLE &&
            (uint32_t)iter->addr == 0x100000) {
            // skip the first 4M physical memory
            uint32_t mm_beg = (uint32_t)iter->addr + 0x300000,
                     mm_end = (uint32_t)iter->addr + iter->len;

            // locate mem_map
            mem_map = (uint8_t (*)[PAGE_SIZE])mm_beg;
            free_pages = std::min((mm_end - mm_beg) / PAGE_SIZE, RAM_MAXPAGE);
            max_free_pages = free_pages;

            // fill free_area with trunk pages
            for (int order = 10, cur = 0; order >= 0; --order) {
                while (cur + (1UL << order) - 1 < free_pages) {
                    auto p = (struct page *)(P2V(mem_map + cur));
                    INIT_LIST_HEAD(&p->list);
                    p->order = order;
                    free_area[order].insert(&p->list);
                    cur += (1UL << order);
                }
            }
            break;
        }
    }
}

int zone_t::find_order(int size) {
    size += sizeof(int);
    for (int ret = 0; ret <= 10; ++ret)
        if ((1UL << ret) * PAGE_SIZE >= static_cast<uint32_t>(size))
            return ret;
    return -1;
}

void *zone_t::alloc(int alloc_size) {
    int order = find_order(alloc_size);

    if (order == -1)
        return nullptr;

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
    uint32_t size = (1UL << cur_order) * PAGE_SIZE;
    while (cur_order > order) {
        --cur_order;
        free_area[cur_order].insert(&p->list);
        p->order = cur_order;
        p = (struct page *)((int32_t)p + (size >>= 1));
    }
    *(int32_t *)p = order;
    return (int32_t*)p + 1;
}

void zone_t::dealloc(void *_p) {
    auto is_buddy = [&](struct page *p, int order) {
        return static_cast<uint32_t>((uint8_t(*)[PAGE_SIZE])p - mem_map) <
                   max_free_pages &&
               p->order == order && !list_isolated(&p->list);
    };

    if (!_p)
        return;

    int order = *((int32_t*)_p - 1);
    free_pages += (1UL << order);

    auto p = (struct page *)((int32_t *)_p - 1);
    uint32_t page_idx = (uint8_t (*)[PAGE_SIZE])p - mem_map, buddy_idx = 0;
    //INIT_LIST_HEAD(&p->list);
    //p->order = 0;
    while (order < 10) {
        buddy_idx = page_idx ^ (1UL << order);
        auto buddy = (struct page *)(mem_map + buddy_idx);
        if (!is_buddy(buddy, order))
            break;
        free_area[order].remove(&buddy->list);
        buddy->order = 0;
        page_idx &= buddy_idx;
        p = (struct page *)(mem_map + page_idx);
        ++order;
    }
    p->order = order;
    free_area[order].insert(&p->list);
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

