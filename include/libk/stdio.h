/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdarg.h>
#ifndef KERNEL_STDIO_H
#define KERNEL_STDIO_H

void k_putc(char c, int fd);
void k_puts(const char *s, int fd);

/* All of these defaults to STDIO */

void k_vsprintf(char *buf, const char *fmt, va_list args);
void k_printf(const char *fmt, ...);
#endif