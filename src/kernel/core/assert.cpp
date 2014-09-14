#include "assert.h"

void print_seg_status() {
    //static int times = 0;
    uint16_t cs, ds, es, ss;

    __asm__ __volatile__("mov %%cs, %0" : "=m"(cs));
    __asm__ __volatile__("mov %%ds, %0" : "=m"(ds));
    __asm__ __volatile__("mov %%es, %0" : "=m"(es));
    __asm__ __volatile__("mov %%ss, %0" : "=m"(ss));

    using namespace std;
    cout /*<< dec << times*/ << ": @ring" << (cs & 0x3) << endl;
    cout /*<< dec << times*/ << ": cs = " << hex << cs << endl;
    cout /*<< dec << times*/ << ": ds = " << hex << ds << endl;
    cout /*<< dec << times*/ << ": es = " << hex << es << endl;
    cout /*<< dec << times*/ << ": ss = " << hex << ss << endl;
    //++times;
}

static void print_stack_trace() {
    uint32_t *ebp, *eip;
    extern multiboot_info_t *glb_mboot_ptr;
    Elf_info elf(glb_mboot_ptr);

    using namespace std;
    __asm__ __volatile__("mov %%ebp, %0" : "=r"(ebp));
    while (ebp) {
        eip = ebp + 1; // 返回地址
        cout << " -> [ " << hex << *eip << " ] " << elf.lookup_symbol(*eip) << endl;
        ebp = (uint32_t *)*ebp;
    }
}

void kernel_panic() {
    using namespace std;
    cout << std::RED << "*** kernel panic ***" << endl;
    print_stack_trace();
    cout << std::RED << "***" << endl;
}
