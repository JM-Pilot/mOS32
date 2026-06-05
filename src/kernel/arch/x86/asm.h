/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#ifndef ASM_H
#define ASM_H

struct registers {
	uint32_t eax, ebx, ecx, edx;
	uint32_t edi, esi, ebp, esp, eip;
	uint32_t eflags;
	uint16_t cs, ds, es, fs, gs, ss;
};


extern void asm_restore_registers(struct registers *r);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void outw(uint16_t port, uint16_t val);
uint16_t inw(uint16_t port);

#endif