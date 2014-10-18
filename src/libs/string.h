#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
void *memcpy(void *, const void *, size_t);
int memcmp(const void *, const void *, size_t);

size_t strlen(const char *);
int strcmp(const char *, const char *);
char *strcpy(char *, const char *);
char *stpcpy(char *, const char *);
char *strcat(char *, const char *);

#ifdef __cplusplus
}
#endif
