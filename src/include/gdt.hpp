#ifndef _YOUMIX_GDT_H_
#define _YOUMIX_GDT_H_

#include <stdint.h>
#include <stddef.h>


namespace GDT {

/*
 * Be cautious about this.
 */
struct SegmentDescriptor {
    /*
     * base0_15, base16_23, base24_31 => Segment base address.
     * limit0_15, limit16_19 => Segment limit
     */
    uint16_t limit0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t access;
    // Desctiptor (0 == system; 1 = code or data)
    // Desctiptor privilege level (0 - 3) *2 bits*
    // Segment present
    // Available for use by system software
    // 64-bit code segment(IA-32e mode only)
    // Default operation size(0 = 16-bit segment; 1 = 32-bit segment)
    // Granularity: 0 = 1 byte; 1 = 4Kbytes

    uint8_t limit16_19 : 4;
    uint8_t other : 4;

    uint8_t base24_31;
} __attribute__((packed));

struct GDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init_gdt();
} // namespace GDT

#ifdef __cplusplus
extern "C"
#endif
void init_gdt();

#endif
