#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

void *memmove(void *dest, const void *src, std::size_t n);

void *memset(void *s, int, std::size_t);

void *memcpy(void *dest, const void *src, std::size_t n);

std::size_t strlen(const char*);

int strcmp(const char *str1, const char *str2);

char *strcpy(char *dest, const char *src);

char *stpcpy(char *dest, const char *src);

char *strcat(char *dest, const char *src);

#ifdef __cplusplus
}
#endif

