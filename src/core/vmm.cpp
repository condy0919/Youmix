#include "../include/vmm.hpp"
#include "../include/ostream.hpp"

__attribute__((aligned(PAGE_SIZE))) uint32_t page_directory[1024];

__attribute__((aligned(PAGE_SIZE))) uint32_t
    page_table_entry[PAGE_TABLE_ENTRY_COUNT][1024];


void init_page() {
    uint32_t kbase = PD_IDX(KERNEL_VIRTUAL_BASE) + 1;

    for (uint32_t i = kbase; i < PAGE_TABLE_ENTRY_COUNT + kbase; ++i) {
        page_directory[i] =
            V2P(page_table_entry[i - kbase]) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    uint32_t *p = (uint32_t *)page_table_entry;
    for (uint32_t i = 0; i < sizeof(page_table_entry) / sizeof(uint32_t); ++i) {
        p[i] = ((i << 12) + 0x400000) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    register_interrupt_handler(14, page_fault);
}

void map(uint32_t *pd, uint32_t va, uint32_t pa, uint32_t flags) {
    extern zone_t zone;

    uint32_t pd_idx = PD_IDX(va), pte_idx = PTE_IDX(va);
    uint32_t *pte = (uint32_t *)(pd[pd_idx] & PAGE_MASK);

    if (!pte) {
        // FIXME: BUGS
        struct page *page = zone.allocate(0);
        pte = (uint32_t *)page->p_addr;
        page->v_addr = va;
        pd[pd_idx] = page->p_addr | PAGE_PRESENT | PAGE_WRITABLE;
        memset(pte, 0, PAGE_SIZE);
    }

    pte[pte_idx] = (pa & PAGE_MASK) | flags;

    __asm__ __volatile__("invlpg (%0)" : : "a"(va));
}

void unmap(uint32_t *pd, uint32_t va) {
    uint32_t pd_idx = PD_IDX(va), pte_idx = PTE_IDX(va);
    uint32_t *pte = (uint32_t *)(pd[pd_idx] & PAGE_MASK);
    
    if (!pte)
        return;
    pte[pte_idx] = 0;

    __asm__ __volatile__("invlpg (%0)" : : "a"(va));
}

uint32_t get_mapping(uint32_t *pd, uint32_t va) {
    uint32_t pd_idx = PD_IDX(va), pte_idx = PTE_IDX(va);
    uint32_t *pte = (uint32_t *)(pd[pd_idx] & PAGE_MASK);

    if (!pte)
        return 0;
    return pte[pte_idx];
}

void page_fault(Register *regs) {
    uint32_t cr2;
    __asm__ __volatile__("movl %%cr2, %0" : "=r"(cr2));

    using namespace io;
    cout << hex << "Page fault at " << regs->eip << ", virtual fault address " << cr2 << endl;
    cout << "Error " << regs->errno << " => ";

    cout << "(";
    if (regs->errno & 0x1)
        cout << " Present";
    cout << ((regs->errno & 0x2) ? " Write" : " Read");
    cout << ((regs->errno & 0x4) ? " User" : " Kernel");
    if (regs->errno & 0x8)
        cout << " Reversed";
    if (regs->errno & 0x10)
        cout << " Instruction";
    cout << " )" << endl;

    while (true)
        ;
}
