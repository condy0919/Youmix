#pragma once

#include <type_traits>
#include <cstdint>
#include <cctype>

#include "algorithm.hpp"
#include "string.hpp"

namespace console {
enum Color {
    black = 0,
    blue = 1,
    green = 2,
    cyan = 3,
    red = 4,
    magenta = 5,
    brown = 6,
    light_grey = 7,
    dark_grey = 8,
    light_blue = 9,
    light_green = 10,
    light_cyan = 11,
    light_red = 12,
    light_magenta = 13,
    light_brown = 14,
    white = 15
};

enum Fmtflags {
    fmt_left = 1 << 0,
    fmt_right = 1 << 1,
    fmt_oct = 1 << 2,
    fmt_dec = 1 << 3,
    fmt_hex = 1 << 4
};

class stream;

stream &left(stream &);
stream &right(stream &);

stream &oct(stream &);
stream &dec(stream &);
stream &hex(stream &);

stream &endl(stream &); // 换行，flush流. 这里会将width置0
                        // 将color变为黑底白字

struct _Setw {
    std::size_t w;
};

_Setw setw(std::size_t);

class stream {
public:
    stream();
    stream(stream &) = delete;
    stream &operator=(stream &) = delete;

    /*
     * INTERFACE
     */
    void clear();
    void scroll(std::size_t = 1);
    stream &put(char, Color = black, Color = white);

    // get/set format flags
    std::uint32_t flags() const;
    std::uint32_t flags(std::uint32_t);

    // set/unset attribute
    std::uint32_t unset(std::uint32_t);
    std::uint32_t set(std::uint32_t);

    // get/set colors
    Color bg_color() const;
    Color bg_color(Color);
    Color fg_color() const;
    Color fg_color(Color);

    // get/set field width
    std::size_t width() const;
    std::size_t width(std::size_t);

    stream &operator<<(char);
    stream &operator<<(std::int32_t);
    stream &operator<<(std::uint32_t);
    stream &operator<<(const char *);

    stream &operator<<(_Setw); // for setw

    stream &operator<<(stream &(*)(stream &)); // for base and adjust

    stream &operator<<(Color); // for foreground color

private:
    template <typename Integer> void itoa(Integer x, char *buf) {
        const char *digits = "9876543210123456789ABCDEF";
        const char *zero = digits + 9;

        Integer t = x;
        std::size_t base =
            (_flags & fmt_oct) ? 8 : ((_flags & fmt_dec) ? 10 : 16);
        char *p = buf;
        while (t) {
            *p++ = zero[t % base];
            t /= base;
        }
        if (x < 0) {
            *p++ = '-';
        } else if (base == 16) {
            *p++ = 'x';
            *p++ = '0';
        } else if (base == 8) {
            *p++ = '0';
        }
        std::reverse(buf, p);
    }

private:
    std::size_t _width;
    std::uint32_t _flags;
    Color _bg, _fg;

    class screen {
    public:
        void clear() {
            std::fill_n(vbuf, MAXROW * MAXCOL, blank);
            row = col = 0;
            move();
        }

        void scroll(std::size_t lines = 1) {
            lines = std::min(lines, static_cast<std::size_t>(MAXROW));
            std::uint16_t(*p)[MAXCOL] = (std::uint16_t(*)[MAXCOL])vbuf;
            for (int i = lines; i < MAXROW; ++i)
                std::copy_n(p[i], MAXCOL, p[i - lines]);
            for (int i = MAXROW - lines; i < MAXROW; ++i)
                std::fill_n(p[i], MAXCOL, blank);
            row -= lines;
            move();
        }

        void putc(char ch, Color bg, Color fg) {
            std::uint16_t cl = (bg << 4) | fg;
            std::uint16_t(*p)[MAXCOL] = (std::uint16_t(*)[MAXCOL])vbuf;

            if (ch == '\b' && col > 0) {
                --col;
            } else if (ch == '\t') {
                col = (col + 4) & ~(4 - 1); // 到达4的整数倍处
            } else if (ch == '\r') {
                col = 0;
            } else if (ch == '\n') {
                col = 0;
                ++row;
            } else if (ch >= ' ') {
                p[row][col] = (cl << 8) | ch;
                ++col;
            }

            if (col == MAXCOL) {
                col = 0;
                ++row;
            }
            if (row == MAXROW)
                scroll();
            move();
        }

    private:
        const std::uint8_t MAXROW = 25, MAXCOL = 80;
        std::uint16_t *const vbuf = (std::uint16_t * const)0xB8000;
        const std::uint16_t blank = (((black << 4) | (white)) << 8) | ' ';

        std::uint8_t row, col;

        void outb(std::uint16_t port, std::uint8_t value) {
            __asm__ __volatile__("outb %1, %0" : : "dN"(port), "a"(value));
        }

        std::uint8_t inb(std::uint16_t port) {
            std::uint8_t ret;
            __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "dN"(port));
            return ret;
        }

        std::uint16_t inw(std::uint16_t port) {
            std::uint16_t ret;
            __asm__ __volatile__("inw %1, %0" : "=a"(ret) : "dN"(port));
            return ret;
        }

        void move() {
            uint16_t loc = row * MAXCOL + col;
            outb(0x3D4, 14);
            outb(0x3D5, loc >> 8);
            outb(0x3D4, 15);
            outb(0x3D5, loc);
        }
    } _screen;
};

} // console

