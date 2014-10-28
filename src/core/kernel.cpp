#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

#include "../include/multiboot.h"
#include "../include/ostream.hpp"
#include "../include/assert.hpp"
#include "../include/timer.hpp"
//#include "../include/gdt.hpp"
//#include "../include/idt.hpp"
#include "../include/pmm.hpp"
#include "../include/vmm.hpp"

/* Check if the compiler thinks if we are targeting the wrong operating system.
 */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

multiboot_info_t *glb_mboot_ptr;

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
int kernel_main(/*uint32_t magic, multiboot_info_t *mb*/) {
    //(void)magic;
    //(void)mb;
    //glb_mboot_ptr = mb;

    //init_gdt();
    //init_idt();

    using namespace io;

    cout.clear();
    cout << "Hello, World" << endl;
    cout << GREEN << "Pachouli GO!" << endl;
    //cout << left << setw(12) << 0xff << " JustDoIt" << endl;
    //cout << hex << 100 << dec << 99 << setw(4) << 0 << endl;
    //cout.printf("%d %c %s\n", -1, 'b', "Konpaku Youmu");
    //(cout << RED << "RWBY\n").puts("This is a newline\n");
    //cout << "Still RED" << endl;
    //cout << "Normal Color" << endl;
    //cout << BROWN << "Yeah!" << endl;
    //cout << hex << magic << " " << mb << endl;
    //cout << dec << 0 << endl;
    
    //cout << glb_mboot_ptr << endl;

    //cout << hex << mb->mem_upper << dec << endl;

    assert(1);

    //__asm__ __volatile__("int $0x3");
    //__asm__ __volatile__("int $0x4");

    //init_timer(200);
    //__asm__ __volatile__("sti"); // Just for timer.
    memory_layout();

    extern zone_t zone;

    cout << "available pages before test: " << zone.free_pages << endl;
    for (int i = 0; i < 100; ++i) {
        void *store[11];
        for (int order = 0; order <= 10; ++order) {
            store[order] = zone.alloc(999);
            *(int*)store[order] = 10;
        }
        for (int order = 0; order <= 10; ++order)
            zone.dealloc(store[order]);
    }
    cout << "available pages after test: " << zone.free_pages << endl;

    // It should cause page fault.
    int *p = (int *)0xffffffff;
    *p = 10;

    return 0;
}
