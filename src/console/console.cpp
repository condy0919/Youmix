#include "console.hpp"

namespace console {
stream::stream()
    : _width(0), _flags(fmt_right | fmt_dec), _bg(black), _fg(white) {}

void stream::clear() { _screen.clear(); }

void stream::scroll(std::size_t lines) { _screen.scroll(lines); }

stream &stream::put(char ch, Color bg, Color fg) {
    _screen.putc(ch, bg, fg);
    return *this;
}

std::uint32_t stream::flags() const { return _flags; }

std::uint32_t stream::flags(std::uint32_t _f) { return _flags = _f; }

std::uint32_t stream::set(std::uint32_t attr) { return flags(flags() | attr); }

std::uint32_t stream::unset(std::uint32_t mask) {
    return flags(flags() & ~mask);
}

Color stream::bg_color() const { return _bg; }

Color stream::bg_color(Color bg) { return _bg = bg; }

Color stream::fg_color() const { return _fg; }

Color stream::fg_color(Color fg) { return _fg = fg; }

std::size_t stream::width() const { return _width; }

std::size_t stream::width(std::size_t w) { return _width = w; }

stream &stream::print(const char *fmt, ...) {
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
        default:
            break;
        }
        ++fmt;
    }
    va_end(ap);
    return *this;
}

stream &stream::operator<<(stream &(*fn)(stream &)) { return fn(*this); }

stream &stream::operator<<(Color fg) {
    _fg = fg;
    return *this;
}

stream &stream::operator<<(_Setw w) {
    _width = w.w;
    return *this;
}

stream &stream::operator<<(const char ch) {
    const char buf[2] = { ch, '\0' };
    return *this << buf;
}

stream &stream::operator<<(long x) {
    char buf[32] = { '\0' };
    itoa(x, buf);
    return *this << buf;
}

stream &stream::operator<<(unsigned long x) {
    char buf[32] = { '\0' };
    itoa(x, buf);
    return *this << buf;
}

stream &stream::operator<<(int x) {
    if (flags() & (fmt_oct | fmt_hex))
        return *this << static_cast<long>(static_cast<unsigned int>(x));
    return *this << static_cast<long>(x);
}

stream &stream::operator<<(unsigned int x) {
    return *this << static_cast<unsigned long>(x);
}

stream &stream::operator<<(const char *s) {
    std::size_t outlen = strlen(s);
    auto prs = [&](const char *s) {
        while (*s)
            put(*s++, _bg, _fg);
    };
    auto prcn = [&](char ch, std::size_t n) {
        while (n--)
            put(ch, _bg, _fg);
    };

    if (_width > outlen) {
        if (_flags & fmt_right) {
            prcn(' ', _width - outlen);
            prs(s);
        } else {
            prs(s);
            prcn(' ', _width - outlen);
        }
    } else {
        prs(s);
    }
    return *this;
}

stream &left(stream &s) {
    s.unset(fmt_right);
    s.set(fmt_left);
    return s;
}

stream &right(stream &s) {
    s.unset(fmt_left);
    s.set(fmt_right);
    return s;
}

stream &oct(stream &s) {
    s.unset(fmt_dec | fmt_hex);
    s.set(fmt_oct);
    return s;
}

stream &dec(stream &s) {
    s.unset(fmt_oct | fmt_hex);
    s.set(fmt_dec);
    return s;
}

stream &hex(stream &s) {
    s.unset(fmt_oct | fmt_dec);
    s.set(fmt_hex);
    return s;
}

stream &endl(stream &s) {
    s.put('\n');
    // s << '\n';
    s.width(0);
    s.bg_color(black);
    s.fg_color(white);
    return s;
}

_Setw setw(std::size_t w) {
    return { w };
}
}
