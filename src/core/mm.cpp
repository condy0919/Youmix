#include "../include/mm.hpp"
#include "../include/ostream.hpp"

extern multiboot_info_t *glb_mboot_ptr;

namespace Memory {
__attribute__((aligned(PAGE_SIZE))) uint32_t page_directory[1024];

__attribute__((aligned(PAGE_SIZE))) uint32_t
    page_table_entry[PAGE_TABLE_ENTRY_COUNT][1024];

zone_t zone;
kheap heap;

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
    uint32_t mmap_addr = ::glb_mboot_ptr->mmap_addr;
    uint32_t mmap_len = ::glb_mboot_ptr->mmap_length;

    auto *beg = (multiboot_mmap_entry *)mmap_addr,
         *end = beg + mmap_len / sizeof(*beg);
    for (auto iter = beg; iter != end; ++iter) {
        if (iter->type == MULTIBOOT_MEMORY_AVAILABLE &&
            (uint32_t)iter->addr == 0x100000) {
            // skip the first 4M physical memory
            uint32_t mm_beg = (uint32_t)iter->addr + 0x300000,
                     mm_end = (uint32_t)iter->addr + iter->len;

            // locate mem_map
            mem_map = (uint8_t(*)[PAGE_SIZE])mm_beg;
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
        p->order = cur_order;
        free_area[cur_order].insert(&p->list);
        p = (struct page *)((uint32_t)p + (size >>= 1));
    }
    set_order(p, order);
    return p;
}

void zone_t::dealloc(void *_p) {
    auto is_present = [](struct page *p) {
        uint32_t addr = (uint32_t)p;
        uint32_t pd_idx = PD_IDX(addr), pte_idx = PTE_IDX(addr);
        uint32_t *pte = (uint32_t *)(page_directory[pd_idx] & PAGE_MASK);
        return pte[pte_idx] & PAGE_PRESENT;
    };
    auto is_buddy = [&](struct page *p, int order) {
        return uint32_t((uint8_t(*)[PAGE_SIZE])p - mem_map) < max_free_pages &&
               is_present(p) && p->order == order && !list_isolated(&p->list);
    };

    if (!_p)
        return;

    int order = get_order(_p);
    free_pages += (1UL << order);

    auto p = (struct page *)_p;
    uint32_t page_idx = (uint8_t (*)[PAGE_SIZE])p - mem_map, buddy_idx = 0;
    while (order < 10) {
        buddy_idx = page_idx ^ (1UL << order);
        auto buddy = (struct page *)(mem_map + buddy_idx);
        if (!is_buddy(buddy, order))
            break;
        free_area[order].remove(&buddy->list);
        page_idx &= buddy_idx;
        p = (struct page *)(mem_map + page_idx);
        ++order;
    }
    p->order = order;
    free_area[order].insert(&p->list);
}

void memory_layout() {
    uint32_t mmap_addr = ::glb_mboot_ptr->mmap_addr;
    uint32_t mmap_len = ::glb_mboot_ptr->mmap_length;

    auto *beg = (multiboot_mmap_entry *)mmap_addr,
         *end = beg + mmap_len / sizeof(*beg);

    // io::cout << "mmap_len = " << io::dec << mmap_len << io::endl;
    for (auto iter = beg; iter != end; ++iter)
        io::cout << "base_addr = " << io::hex << io::setw(16) << iter->addr
                 << io::setw(9) << " length = " << iter->len
                 << " type = " << io::setw(0) << io::dec << iter->type
                 << io::endl;
}

int zone_t::find_order(int size) {
    // Try to fuck the page table entry.
    for (int ret = 0; ret <= 10; ++ret)
        if ((1UL << ret) * PAGE_SIZE >= static_cast<uint32_t>(size))
            return ret;
    return -1;
}

/*
 * The following 2 operations are safe only when CR4.PGE is not set
 */
int zone_t::get_order(void *va) {
    uint32_t addr = (uint32_t)va;
    uint32_t pd_idx = PD_IDX(addr), pte_idx = PTE_IDX(addr);
    uint32_t *pte = (uint32_t *)(page_directory[pd_idx] & PAGE_MASK);
    int ret = ((pte[pte_idx] & (0xf << 8)) >> 8);
    return ret;
}

void zone_t::set_order(void *va, uint8_t order) {
    uint32_t addr = (uint32_t)va;
    uint32_t pd_idx = PD_IDX(addr), pte_idx = PTE_IDX(addr);
    uint32_t* pte = (uint32_t*)(page_directory[pd_idx] & PAGE_MASK);
    pte[pte_idx] &= ~(0xf << 8);
    pte[pte_idx] |= (order << 8);
}


void init_page_dir() {
    uint32_t kbase = PD_IDX(KERNEL_VIRTUAL_BASE) + 1;

    for (uint32_t i = kbase; i < PAGE_TABLE_ENTRY_COUNT + kbase; ++i) {
        page_directory[i] =
            V2P(page_table_entry[i - kbase]) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    uint32_t *p = (uint32_t *)page_table_entry;
    for (uint32_t i = 0; i < sizeof(page_table_entry) / sizeof(uint32_t); ++i) {
        p[i] = ((i << 12) + 0x400000) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    IDT::register_interrupt_handler(14, page_fault);
}

void map(uint32_t va, uint32_t pa, uint32_t flags) {
    uint32_t *pd = page_directory;
    uint32_t pd_idx = PD_IDX(va), pte_idx = PTE_IDX(va);
    uint32_t *pte = (uint32_t *)(pd[pd_idx] & PAGE_MASK);

    if (!pte) {
        void *page = zone.alloc(PAGE_SIZE);
        pte = (uint32_t *)page;
        pd[pd_idx] = (uint32_t)pte | PAGE_PRESENT | PAGE_WRITABLE;
        memset(pte, 0, PAGE_SIZE);
    }

    pte[pte_idx] = (pa & PAGE_MASK) | flags;

    // Clobber memory to avoid optimizer re-ordering access before invlpg,
    // which may cause nasty bugs.
    __asm__ __volatile__("invlpg (%0)" : : "r"(va) : "memory");
}

void unmap(uint32_t va) {
    uint32_t *pd = page_directory;
    uint32_t pd_idx = PD_IDX(va), pte_idx = PTE_IDX(va);
    uint32_t *pte = (uint32_t *)(pd[pd_idx] & PAGE_MASK);
    
    if (!pte)
        return;
    pte[pte_idx] = 0;

    __asm__ __volatile__("invlpg (%0)" : : "r"(va) : "memory");
}

uint32_t get_mapping(uint32_t va) {
    uint32_t *pd = page_directory;
    uint32_t pd_idx = PD_IDX(va), pte_idx = PTE_IDX(va);
    uint32_t *pte = (uint32_t *)(pd[pd_idx] & PAGE_MASK);

    if (!pte)
        return 0;
    return pte[pte_idx] & PAGE_MASK;
}

void page_fault(IDT::Register *regs) {
    uint32_t cr2;
    __asm__ __volatile__("movl %%cr2, %0" : "=r"(cr2));

    using namespace io;
    cout << hex << "Page fault at " << regs->eip << ", virtual fault address " << cr2 << endl;
    cout << "Error " << regs->errno << " => ";

    cout << "(";
    cout << ((regs->errno & 0x1) ? " Present" : " NotPresent");
    cout << ((regs->errno & 0x2) ? " Write" : " Read");
    cout << ((regs->errno & 0x4) ? " User" : " Kernel");
    if (regs->errno & 0x8)
        cout << " Reversed";
    if (regs->errno & 0x10)
        cout << " Instruction";
    cout << " )" << endl;

    __asm__ __volatile__("hlt");
}

cache_cache::cache_cache(int _s)
    : block_size(_s), list(LIST_HEAD_INIT(list)) {}

struct list_head *cache_cache::insert(struct list_head *p) {
    list_add(p, &list);
    return p;
}

struct list_head *cache_cache::remove(struct list_head *p) {
    auto ret = p->prev;
    list_del(p);
    return ret;
}

void *cache_cache::front() {
    return list.next;
}

void cache_cache::pop_front() {
    remove(list.next);
}

void *cache_cache::alloc() {
    if (empty())
        refill();
    void *ret = front();
    pop_front();
    return ret;
}

void cache_cache::dealloc(void *va) {
    struct list_head *p = (struct list_head *)va;
    insert(p);
}

void cache_cache::refill() {
    void *page = zone.alloc(PAGE_SIZE);
    for (uint32_t i = 0; i < PAGE_SIZE; i += block_size + sizeof(uint32_t))
        dealloc((uint8_t *)page + i);
}

void *kheap::alloc(uint32_t size) {
    uint32_t i = 0;

    if (size == 0)
        return nullptr;

#define CACHE(x) \
    if (size > ((x) - sizeof(uint32_t))) \
        ++i; \
    else \
        goto fin;
#include "../include/kcache_size.hpp"
#undef CACHE

fin:
    if (i == sizeof(cache) / sizeof(cache[0])) {
        return zone.alloc(size);
    } else {
        uint32_t *ret = (uint32_t *)cache[i].alloc();
        *ret = i;
        return ret + 1;
    }
}

void kheap::dealloc(void *va) {
    if (((uint32_t)va & (PAGE_SIZE - 1)) == 0) {
        zone.dealloc(va);
    } else {
        uint32_t *addr = (uint32_t *)va - 1, i = *addr;
        cache[i].dealloc(addr);
    }
}

} // namespace Memory

