#ifndef _YOUMIX_SCREEN_H_
#define _YOUMIX_SCREEN_H_

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "asm.hpp"
#include "vmm.hpp"
#include "../libs/algorithm"
#include "../libs/string.h"

namespace io {
enum Color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MEGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15
};
}

class screen {
public:
    screen();
    void clear();
    void scroll();
    void puts(const char *);
    void putc(const char);
    void gotoxy(uint8_t, uint8_t);
    
    io::Color bg_color() const;
    io::Color bg_color(io::Color);
    io::Color fg_color() const;
    io::Color fg_color(io::Color);

private:
    const uint8_t MAXROW = 25, MAXCOL = 80;
    uint16_t *const vbuf = (uint16_t * const)(0xB8000 /*+ KERNEL_VIRTUAL_BASE*/);
    const uint16_t BLANK = (((io::BLACK << 4) | io::WHITE) << 8) | ' ';
    uint8_t row, col;
    uint16_t color;

    void move() {
        uint16_t loc = row * MAXCOL + col;
        outb(0x3D4, 14);
        outb(0x3D5, loc >> 8);
        outb(0x3D4, 15);
        outb(0x3D5, loc);
    }
};

#endif
