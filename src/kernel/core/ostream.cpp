#include "ostream.hpp"

namespace std {
ostream cout;

ostream::ostream() : _width(0), _flags(fmt_right | fmt_dec) {}

uint32_t ostream::flags() const { return _flags; }
uint32_t ostream::flags(uint32_t _f) { return _flags = _f; }

uint32_t ostream::set(uint32_t attr) { return flags(flags() | attr); }
uint32_t ostream::unset(uint32_t attr) { return flags(flags() & ~attr); }

size_t ostream::width() const { return _width; }
size_t ostream::width(size_t w) { return _width = w; }

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
        case 'c': {
            char c = va_arg(ap, int);
            *this << c;
            break;
        }
        case 'p': {
            void *p = va_arg(ap, void *);
            *this << p;
            break;
        }

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

ostream &ostream::operator<<(const char *s) {
    size_t outlen = strlen(s);
    auto prcn = [&](char ch, size_t n) {
        while (n-- > 0)
            putc(ch);
    };
    if (width() > outlen) {
        if (flags() & fmt_right) {
            prcn(' ', width() - outlen);
            puts(s);
        } else {
            puts(s);
            prcn(' ', width() - outlen);
        }
    } else {
        puts(s);
    }
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
    return s;
}

_Setw setw(size_t n) { return {n}; }
}
