/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#ifndef ASM_H
#define ASM_H

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void outw(uint16_t port, uint16_t val);
uint16_t inw(uint16_t port);

#endif