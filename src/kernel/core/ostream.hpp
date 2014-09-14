#pragma once

#include "screen.hpp"
#include <stdarg.h>

struct _Setw {
    size_t _w;
};

enum Fmtflags {
    fmt_left = 1 << 0,
    fmt_right = 1 << 1,
    fmt_oct = 1 << 2,
    fmt_dec = 1 << 3,
    fmt_hex = 1 << 4
};

namespace std {
class ostream : public screen {
public:
    ostream();
    ostream(ostream &) = delete;
    ostream &operator=(ostream &) = delete;

    uint32_t flags() const;
    uint32_t flags(uint32_t);

    uint32_t unset(uint32_t);
    uint32_t set(uint32_t);

    size_t width() const;
    size_t width(size_t);

    // C-style print
    ostream &printf(const char *, ...);

    ostream &operator<<(const char);
    ostream &operator<<(long);
    ostream &operator<<(unsigned long);
    ostream &operator<<(int);
    ostream &operator<<(unsigned int);
    ostream &operator<<(const char *);

    ostream &operator<<(ostream &(*)(ostream &)); // for base and adjust
    ostream &operator<<(_Setw);                   // for setw
    ostream &operator<<(Color);                   // for foreground color

    template <typename T> ostream &operator<<(const T *p) {
        uint32_t temp = flags();
        set(fmt_hex);
        *this << (unsigned long)p;
        flags(temp);
        return *this;
    }

private:
    template <typename Integral> void itoa(Integral x, char *buf) {
        const char *digits = "9876543210123456789ABCDEF";
        const char *zero = digits + 9;

        Integral t = x;
        int base = (_flags & fmt_oct) ? 8 : ((_flags & fmt_dec) ? 10 : 16);
        char *p = buf;
        do {
            *p++ = zero[t % base];
            t /= base;
        } while (t);
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
    size_t _width;
    uint32_t _flags;
};

ostream &left(ostream &);
ostream &right(ostream &);

ostream &oct(ostream &);
ostream &dec(ostream &);
ostream &hex(ostream &);

ostream &endl(ostream &);

_Setw setw(size_t n);

extern ostream cout;
}
