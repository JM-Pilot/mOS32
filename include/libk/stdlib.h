/* MIT LICENSE (C) 2026 JM-Pilot */
#ifndef KERNEL_STDLIB_H
#define KERNEL_STDLIB_H

#define BASE_HEX 16
#define BASE_DEC 10
#define BASE_BIN 2
/* No octal because who even uses that */

char *k_itoa(char *buf, int val, int base);
#endif