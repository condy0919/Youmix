#include "string.hpp"

// CRAP
void *memmove(void *dest, const void *src, std::size_t n) {
    register char *_dst = (char *)dest;
    register char *_src = (char *)src;
    for (std::size_t i = 0; i < n; ++i)
        _dst[i] = _src[i];
    return dest;
}

void *memset(void *s, int c, std::size_t n) {
    char ch = c;
    register char *p = (char*)s;
    while (n--)
        *p++ = ch;
    return p;
}

void *memcpy(void *dest, const char *src, std::size_t n) {
    return memmove(dest, src, n);
}

std::size_t strlen(const char* s) {
    register const char *p = s;

    if (*p && (std::uint32_t)p % 0x4)
        ++p;
    if (!*p)
        return p - s;

    if (*p && (std::uint32_t)p % 0x4)
        ++p;
    if (!*p)
        return p - s;

    if (*p && (std::uint32_t)p % 0x4)
        ++p;
    if (!*p)
        return p - s;

    std::uint32_t lomagic = 0x01010101, himagic = 0x80808080;
    for (std::uint32_t *longword = (std::uint32_t *)p;; ++longword) {
        std::uint32_t val = *longword;
        if (((val - lomagic) & himagic) != 0) {
            const char *cp = (const char *)longword;
            if (cp[0] == '\0')
                return cp - s;
            else if (cp[1] == '\0')
                return cp - s + 1;
            else if (cp[2] == '\0')
                return cp - s + 2;
            else
                return cp - s + 3;
        }
    }
    return 0;
}

int strcmp(const char *str1, const char *str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        ++str1;
        ++str2;
    }
    return *str1 - *str2;
}

char *strcpy(char *dest, const char *src) {
    while ((*dest++ = *src++))
        ;
    return dest;
}

char *strcat(char *dest, const char *src) {
    char *t = dest;
    dest += strlen(dest);
    strcpy(dest, src);
    return t;
}

