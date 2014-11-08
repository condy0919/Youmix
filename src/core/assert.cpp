#include "../include/assert.hpp"

void print_seg_status() {
    uint16_t cs, ds, es, ss;

    __asm__ __volatile__("mov %%cs, %0" : "=m"(cs));
    __asm__ __volatile__("mov %%ds, %0" : "=m"(ds));
    __asm__ __volatile__("mov %%es, %0" : "=m"(es));
    __asm__ __volatile__("mov %%ss, %0" : "=m"(ss));

    using namespace io;
    cout << ": @ring" << (cs & 0x3) << endl;
    cout << ": cs = " << hex << cs << endl;
    cout << ": ds = " << hex << ds << endl;
    cout << ": es = " << hex << es << endl;
    cout << ": ss = " << hex << ss << endl;
}

extern multiboot_info_t *glb_mboot_ptr;
namespace {
void print_stack_trace() {
    uint32_t *ebp, *eip;
    Elf_info elf(::glb_mboot_ptr);

    using namespace io;
    __asm__ __volatile__("mov %%ebp, %0" : "=r"(ebp));
    while (ebp) {
        eip = ebp + 1; // 返回地址
        cout << " -> [ " << hex << *eip << " ] " << elf.lookup_symbol(*eip) << endl;
        ebp = (uint32_t *)*ebp;
    }
}

}

void kernel_panic() {
    using namespace io;
    cout << Color::RED << "*** kernel panic ***" << endl;
    print_stack_trace();
    cout << Color::RED << "***" << endl;
}
