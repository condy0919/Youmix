#include "../include/ostream.hpp"

namespace io {
ostream cout;

ostream::ostream() : _width(0), _flags(fmt_right | fmt_dec), _fillch('\0') {}

uint32_t ostream::flags() const { return _flags; }
uint32_t ostream::flags(uint32_t _f) { return _flags = _f; }

uint32_t ostream::set(uint32_t attr) { return flags(flags() | attr); }
uint32_t ostream::unset(uint32_t attr) { return flags(flags() & ~attr); }

size_t ostream::width() const { return _width; }
size_t ostream::width(size_t w) { return _width = w; }

char ostream::fill() const { return _fillch; }
char ostream::fill(char ch) {
    char t = _fillch;
    _fillch = ch;
    return t;
}

ostream &ostream::printf(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    while (*fmt) {
        if (*fmt != '%') {
            *this << *fmt++;
            continue;
        }
        switch (*++fmt) {
        case 'd':
            *this << va_arg(ap, int);
            break;
        case 'u':
            *this << va_arg(ap, unsigned int);
            break;
        case 's':
            *this << va_arg(ap, const char *);
            break;
        case '%':
            *this << '%';
            break;
        case 'x': {
            unsigned int temp = flags();
            *this << hex << va_arg(ap, unsigned int);
            flags(temp);
            break;
        }
        case 'c':
            *this << static_cast<char>(va_arg(ap, int));
            break;
        case 'p':
            *this << va_arg(ap, void *);
            break;
        default:
            break;
        }
        ++fmt;
    }
    va_end(ap);
    return *this;
}

ostream &ostream::operator<<(ostream &(*fn)(ostream &)) { return fn(*this); }

ostream &ostream::operator<<(Color fg) {
    fg_color(fg);
    return *this;
}

ostream &ostream::operator<<(_Setw w) {
    width(w._w);
    return *this;
}

ostream &ostream::operator<<(_Setfill f) {
    fill(f._c);
    return *this;
}

ostream &ostream::operator<<(const char ch) {
    const char buf[2] = {ch, '\0'};
    return *this << buf;
}

ostream &ostream::operator<<(long x) {
    char buf[32] = {'\0'};
    itoa(x, buf);
    return *this << buf;
}

ostream &ostream::operator<<(unsigned long x) {
    char buf[32] = {'\0'};
    itoa(x, buf);
    return *this << buf;
}

ostream &ostream::operator<<(int x) {
    if (flags() & (fmt_oct | fmt_hex))
        return *this << static_cast<long>(static_cast<unsigned int>(x));
    return *this << static_cast<long>(x);
}

ostream &ostream::operator<<(unsigned int x) {
    return *this << static_cast<unsigned long>(x);
}

#ifdef __i386__
ostream &ostream::operator<<(uint64_t x) {
    char buf[32] = {'\0'};
    itoa(x, buf);
    return *this << buf;
}
#endif

ostream &ostream::operator<<(const char *s) {
    size_t outlen = strlen(s);
    auto prcn = [&](char ch, size_t n) {
        while (n-- > 0)
            putc(ch);
    };
    if (width() > outlen) {
        if (flags() & fmt_right) {
            prcn(_fillch ? _fillch : ' ', width() - outlen);
            puts(s);
        } else {
            puts(s);
            prcn(_fillch ? _fillch : ' ', width() - outlen);
        }
    } else {
        puts(s);
    }
    return *this;
}

ostream &ostream::operator<<(const void *p) {
    uint32_t temp = flags();
    *this << hex << (unsigned long)p;
    flags(temp);
    return *this;
}

ostream &left(ostream &s) {
    s.unset(fmt_right);
    s.set(fmt_left);
    return s;
}

ostream &right(ostream &s) {
    s.unset(fmt_left);
    s.set(fmt_right);
    return s;
}

ostream &oct(ostream &s) {
    s.unset(fmt_dec | fmt_hex);
    s.set(fmt_oct);
    return s;
}

ostream &dec(ostream &s) {
    s.unset(fmt_oct | fmt_hex);
    s.set(fmt_dec);
    return s;
}

ostream &hex(ostream &s) {
    s.unset(fmt_oct | fmt_dec);
    s.set(fmt_hex);
    return s;
}

ostream &endl(ostream &s) {
    s.putc('\n');
    s.width(0);
    s.bg_color(BLACK);
    s.fg_color(WHITE);
    s.fill('\0');
    return dec(s); // not compatible with stdandard.
}

_Setw setw(size_t n) { return {n}; }

_Setfill setfill(char _c) { return {_c}; }
}
