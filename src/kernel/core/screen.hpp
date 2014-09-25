#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "asm_port.hpp"
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
    uint16_t *const vbuf = (uint16_t * const)0xB8000;
    const uint16_t BLANK = (((io::BLACK << 4) | io::WHITE) << 8) | ' ';
    uint8_t row, col;
    uint16_t color;

    // ASM helpful code...
    //void outb(uint16_t port, uint8_t value) {
    //    __asm__ __volatile__("outb %1, %0" : : "dN"(port), "a"(value));
    //}

    //uint8_t inb(uint16_t port) {
    //    uint8_t ret;
    //    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "dN"(port));
    //    return ret;
    //}

    //void outw(uint16_t port, uint16_t value) {
    //    __asm__ __volatile__("outw %1, %0" : : "dN"(port), "a"(value));
    //}

    //uint16_t inw(uint16_t port) {
    //    uint16_t ret;
    //    __asm__ __volatile__("inw %1, %0" : "=a"(ret) : "dN"(port));
    //    return ret;
    //}

    void move() {
        uint16_t loc = row * MAXCOL + col;
        outb(0x3D4, 14);
        outb(0x3D5, loc >> 8);
        outb(0x3D4, 15);
        outb(0x3D5, loc);
    }
};
