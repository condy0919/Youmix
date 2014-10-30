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
#include "../include/mm.hpp"
#include "../include/logo.hpp"

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

    Memory::memory_layout();

    cout << "available pages before test: " << Memory::zone.free_pages << endl;
    for (int i = 0; i < 100; ++i) {
        void *store[11];
        for (int order = 0; order <= 10; ++order) {
            store[order] = Memory::zone.alloc(999);
            *(int*)store[order] = 10;
        }
        for (int order = 0; order <= 10; ++order)
            Memory::zone.dealloc(store[order]);
    }
    cout << "available pages after test: " << Memory::zone.free_pages << endl;

    // operator new test
    char *p = new char('a');
    cout << *p << endl;
    delete p;

    void *addr1 = Memory::heap.alloc(50);
    cout << "alloc memory in " << addr1 << endl;
    void *addr2 = Memory::heap.alloc(500);
    cout << "alloc memory in " << addr2 << endl;
    void *addr3 = Memory::heap.alloc(5000);
    cout << "alloc memory in " << addr3 << endl;
    void *addr4 = Memory::heap.alloc(50000);
    cout << "alloc memory in " << addr4 << endl;

    Memory::heap.dealloc(addr1);
    cout << "free memory in " << addr1 << endl;
    Memory::heap.dealloc(addr2);
    cout << "free memory in " << addr2 << endl;
    Memory::heap.dealloc(addr3);
    cout << "free memory in " << addr3 << endl;
    Memory::heap.dealloc(addr4);
    cout << "free memory in " << addr4 << endl;



    // Failed in placement new operator
    //char buf[10];
    //char *foo = new (buf) char('a');
    //cout << *foo << endl;

    //int *p = (int *)Memory::heap.alloc(10);
    //*p = 10;
    //cout << *p << endl;
    //Memory::heap.dealloc(p);

    show_logo();

    // It should cause page fault.
    //p = (char *)0xffffffff;
    //*p = 'a';

    return 0;
}
