#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

void *memmove(void *dest, const void *src, std::size_t n);

std::size_t strlen(const char*);

void *memset(void *s, int, std::size_t);

#ifdef __cplusplus
}
#endif
