#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
#include "ostream.hpp"
#include "assert.hpp"
#include "timer.hpp"
#include "pmm.hpp"
//#include "gdt.hpp"

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
int kernel_main(uint32_t magic, multiboot_info_t *mb) {
    (void)magic;
    (void)mb;
    //glb_mboot_ptr = mb;

    //init_gdt();

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

    assert(1);

    //__asm__ __volatile__("int $0x3");
    //__asm__ __volatile__("int $0x4");

    //init_timer(200);
    //__asm__ __volatile__("sti"); // Just for timer.
    memory_layout();

    extern zone_t zone;

    cout << "available pages before test: " << zone.free_pages << endl;
    for (int i = 0; i < 100; ++i) {
        struct page *store[11];
        for (int order = 0; order <= 10; ++order)
            store[order] = zone.allocate(5);
        for (int order = 0; order <= 10; ++order)
            zone.deallocate(store[order], 5);
    }
    cout << "available pages after test: " << zone.free_pages << endl;

    //auto *p2 = zone.allocate(10);
    //cout << "address: " << p2->p_addr << endl;
    //while (auto *p = zone.allocate(10))
    //    cout << "address: " << p->p_addr << endl;


    return 0;
}
