#ifndef _YOUMIX_OSTREAM_H_
#define _YOUMIX_OSTREAM_H_

#include <stdarg.h>
#include "screen.hpp"

struct _Setw {
    size_t _w;
};

struct _Setfill {
    char _c;
};

enum Fmtflags {
    fmt_left = 1 << 0,
    fmt_right = 1 << 1,
    fmt_oct = 1 << 2,
    fmt_dec = 1 << 3,
    fmt_hex = 1 << 4
};


namespace io {
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

    char fill() const;
    char fill(char);

    // C-style print
    ostream &printf(const char *, ...);

    ostream &operator<<(const char);
    ostream &operator<<(long);
    ostream &operator<<(unsigned long);
    ostream &operator<<(int);
    ostream &operator<<(unsigned int);
    ostream &operator<<(uint64_t);
    ostream &operator<<(const char *);
    ostream &operator<<(const void *);

    ostream &operator<<(ostream &(*)(ostream &)); // for base and adjust
    ostream &operator<<(_Setw);                   // for setw
    ostream &operator<<(_Setfill);                // for setfill
    ostream &operator<<(Color);                   // for foreground color

private:
    template <typename Integral> char *itoa(Integral x, char *buf) {
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
        return p;
    }

private:
    size_t _width;
    uint32_t _flags;
    char _fillch;
};

ostream &left(ostream &);
ostream &right(ostream &);

ostream &oct(ostream &);
ostream &dec(ostream &);
ostream &hex(ostream &);

ostream &endl(ostream &);

_Setw setw(size_t n);
_Setfill setfill(char c);

extern ostream cout;
}

#endif
