#include "../include/screen.hpp"

screen::screen() : row(0), col(0), color((io::BLACK << 4) | io::WHITE) {}

void screen::clear() {
    std::fill_n(vbuf, MAXROW * MAXCOL, BLANK);
    row = col = 0;
    move();
}

void screen::scroll() {
    uint16_t(*p)[MAXCOL] = (uint16_t(*)[MAXCOL])vbuf;
    for (size_t i = 1; i < MAXROW; ++i)
        std::copy_n(p[i], MAXCOL, p[i - 1]);
    std::fill_n(p[MAXROW - 1], MAXCOL, BLANK);
    --row;
    move();
}

void screen::puts(const char *s) {
    while (*s)
        putc(*s++);
}

void screen::putc(const char ch) {
    uint16_t(*p)[MAXCOL] = (uint16_t(*)[MAXCOL])vbuf;

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
        p[row][col] = (color << 8) | ch;
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

void screen::gotoxy(uint8_t x, uint8_t y) {
    row = x;
    col = y;
    move();
}

io::Color screen::bg_color() const { return io::Color(color >> 4); }

io::Color screen::bg_color(io::Color c) {
    return io::Color(color = ((color & 0x00ff) | (c << 4)));
}

io::Color screen::fg_color() const { return io::Color(color & 0xf); }

io::Color screen::fg_color(io::Color c) {
    return io::Color(color = ((color & 0xff00) | c));
}

