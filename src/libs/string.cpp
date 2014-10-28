#include "string.h"

void *memcpy(void *dest, const char *src, size_t n) {
    register char *d = (char *)dest;
    register char *s = (char *)src;
    for (size_t i = 0; i < n; ++i)
        d[i] = s[i];
    return dest;
}

void *memset(void *s, int c, size_t n) {
    long int dstp = (long int)s;
    
    if (n >= 8) {
        uint32_t cccc = (uint8_t)c;
        cccc |= cccc << 8;
        cccc |= cccc << 16;
        while (dstp % sizeof(uint32_t) != 0) {
            *(uint8_t*)dstp = c;
            ++dstp;
            --n;
        }

        size_t xlen = n / (sizeof(uint32_t) * 8);
        while (xlen > 0) {
            uint32_t *p = (uint32_t *)dstp;
            p[0] = cccc;
            p[1] = cccc;
            p[2] = cccc;
            p[3] = cccc;
            p[4] = cccc;
            p[5] = cccc;
            p[6] = cccc;
            p[7] = cccc;
            dstp += 8 * sizeof(uint32_t);
            --xlen;
        }
        n %= 8 * sizeof(uint32_t);
    }

    while (n-- > 0) {
        *(uint8_t *)dstp = c;
        ++dstp;
    }
    return s;
}

// CRAP
void *memmove(void *dest, const char *src, size_t n) {
    return memcpy(dest, src, n);
}

int memcmp(const void *s1, const void *s2, size_t n) {
    register const char *p1 = (const char *)s1;
    register const char *p2 = (const char *)s2;
    for (size_t i = 0; i < n && *p1 == *p2; ++i) {
        ++p1;
        ++p2;
    }
    return *p1 - *p2;
}

size_t strlen(const char *s) {
    register const char *p = s;

    if (*p && (uint32_t)p % 0x4)
        ++p;
    if (!*p)
        return p - s;

    if (*p && (uint32_t)p % 0x4)
        ++p;
    if (!*p)
        return p - s;

    if (*p && (uint32_t)p % 0x4)
        ++p;
    if (!*p)
        return p - s;

    uint32_t lomagic = 0x01010101, himagic = 0x80808080;
    for (uint32_t *longword = (uint32_t*)p; ;++longword) {
        uint32_t val = *longword;
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
    char *ret = dest;
    while ((*dest++ = *src++))
        ;
    return ret;
}

char *stpcpy(char *dest, const char *src) {
    do
        *dest++ = *src++;
    while (*src++ != '\0');
    return dest - 1;
}

char *strcat(char *dest, const char *src) {
    char *t = dest;
    dest += strlen(dest);
    strcpy(dest, src);
    return t;
}

