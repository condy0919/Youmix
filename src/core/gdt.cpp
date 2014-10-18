#include "../include/gdt.hpp"

#define GDTSIZE 8

SegmentDescriptor gdtdesc[GDTSIZE];
GDTPointer gdt_ptr;

void init_gdt() {
    auto fn = [](SegmentDescriptor &sd, uint32_t base, uint32_t limit,
                 uint8_t access, uint8_t other) {
        sd.base0_15 = (base & 0x0000ffff) >> 0;
        sd.base16_23 = (base & 0x00ff0000) >> 16;
        sd.base24_31 = (base & 0xff000000) >> 24;

        sd.limit0_15 = (limit & 0x0000ffff) >> 0;
        sd.limit16_19 = (limit & 0x000f0000) >> 16;

        sd.access = access;
        sd.other = other;
    };

    gdt_ptr.limit = sizeof(gdtdesc);
    gdt_ptr.base = (uint32_t)gdtdesc;

    fn(gdtdesc[0], 0x0, 0x0, 0x0, 0x0);

    fn(gdtdesc[1], 0x0, 0xffffffff, 0x9b, 0x0d); // code
    fn(gdtdesc[2], 0x0, 0xffffffff, 0x93, 0x0d); // data
    fn(gdtdesc[3], 0x0, 0x0, 0x97, 0x0d);        // stack

    fn(gdtdesc[4], 0x0, 0xffffffff, 0xff, 0x0d); // user code
    fn(gdtdesc[5], 0x0, 0xffffffff, 0xf3, 0x0d); // user data
    fn(gdtdesc[6], 0x0, 0x0, 0xf7, 0x0d);        // user stack

    __asm__ __volatile__("lgdtl (gdt_ptr)");

    __asm__ __volatile__("movw $0x10, %ax;"
                         "movw %ax, %ds;" // points to data segment
                         "movw %ax, %es;"
                         "movw %ax, %fs;"
                         "movw %ax, %gs;"
                         "movw %ax, %ss;"
                         "ljmp $0x08, $flush;" // jumps to code segment, forces
                                               // CPU to flush pipeline and
                                               // cache
                         "flush:;");
}
