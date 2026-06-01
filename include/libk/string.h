/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#include <stddef.h>
#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

void *k_memset(void *dest, int val, size_t len);
void *k_memcpy(void *dest, const void *src, size_t len);
void *k_memmove(void *dest, void *src, size_t len);
int k_memcmp(const void *a, const void *b, size_t count);
int k_strcmp(const char *p1, const char *p2);
int k_strncmp(const char *s1, const char *s2, size_t n);

#endif