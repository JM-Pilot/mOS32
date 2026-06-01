/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#include "gdt.h"

struct gdt_r gdtr;
struct gdt_desc gdt[3];

extern void gdt_load();
/* i really got to keep the 80 char limit*/
void gdt_set_desc(int n, uint32_t base,
	uint32_t limit, uint8_t access, uint8_t flags)
{
	gdt[n].base_low = base & 0xFFFF;
	gdt[n].base_mid = (base >> 16 & 0xFF);
	gdt[n].base_high = (base >> 24 & 0xFF);

	gdt[n].access_byte = access;
	gdt[n].flags = flags & 0xF0;
	gdt[n].flags |= ((limit >> 16) & 0xF);
	gdt[n].limit_low = limit & 0xFFFF;
}
void gdt_init(){
	gdtr.size = sizeof(gdt) - 1;
	gdtr.offset = (uint32_t)&gdt;

	gdt_set_desc(0, 0, 0, 0, 0);
	/* Kernel */
	gdt_set_desc(1, 0, 0xFFFFF, 0x9A, 0xCF);
	gdt_set_desc(2, 0, 0xFFFFF, 0x92, 0xCF);

	/* User */
	/* gdt_set_desc(3, 0, 0xFFFFF, 0xFA, 0xCF);
	gdt_set_desc(4, 0, 0xFFFFF, 0xF2, 0xCF);
	*/
	gdt_load();
}