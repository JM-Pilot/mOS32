/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#ifndef IDT_H
#define IDT_H

struct idt_r {
	uint16_t size;
	uint32_t offset;
}__attribute__((packed));

struct idt_desc {
	uint16_t offset_low;
	uint16_t segment;
	uint8_t reserved;
	uint8_t attributes;
	uint16_t offset_high;
}__attribute__((packed));

struct register_frame {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
};


void idt_set_desc(int n, uint32_t offset, 
	uint16_t segment, uint8_t attributes);
void idt_init();
#endif