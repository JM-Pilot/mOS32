/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#include "idt.h"

extern uint32_t isr_stub_table[32];
struct idt_r idtr;
struct idt_desc idt[256];

void idt_set_desc(int n, uint32_t offset, 
	uint16_t segment, uint8_t attributes)
{
	idt[n].offset_low = offset & 0xFFFF;
	idt[n].offset_high = (offset >> 16) & 0xFFFF;
	idt[n].segment = segment;
	idt[n].attributes = attributes;
	idt[n].reserved = 0;
}
void idt_init(){
	idtr.size = sizeof(idt) - 1;
	idtr.offset = (uint32_t)&idt;
	
	for (int i = 0; i < 32; i++)
		idt_set_desc(i, isr_stub_table[i], 0x08, 0x8E);
	asm volatile ("lidt %0" 
		:
		: "m"(idtr));
}