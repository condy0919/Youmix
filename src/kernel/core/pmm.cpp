#include "pmm.hpp"

#include "ostream.hpp"

void memory_layout() {
    extern multiboot_info_t *glb_mboot_ptr;

    uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
    uint32_t mmap_len = glb_mboot_ptr->mmap_length;

    auto *beg = (multiboot_mmap_entry *)mmap_addr,
         *end = beg + mmap_len / sizeof(*beg);
    for (auto iter = beg; iter != end; ++iter)
        io::cout << "base_addr = " << io::hex << io::setw(16) << iter->addr
                 << io::setw(9) << " length = " << iter->len
                 << " type = " << io::setw(0) << io::dec << iter->type
                 << io::endl;
}
