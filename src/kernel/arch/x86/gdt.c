/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#include "gdt.h"

struct gdt_r gdtr;
struct gdt_desc gdt[6];
struct tss_desc tss = {0};

extern void gdt_load();
extern char kernel_stack_top[];
extern char ist1_stack_top[];
extern char ist2_stack_top[];
extern char ist3_stack_top[];
extern char ist4_stack_top[];
extern char ist5_stack_top[];
extern char ist6_stack_top[];
extern char ist7_stack_top[];

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
	tss.rsp0 = (uint64_t)kernel_stack_top;

	tss.ist1 = (uint64_t)ist1_stack_top;
	tss.ist2 = (uint64_t)ist2_stack_top;
	tss.ist3 = (uint64_t)ist3_stack_top;
	tss.ist4 = (uint64_t)ist4_stack_top;
	tss.ist5 = (uint64_t)ist5_stack_top;
	tss.ist6 = (uint64_t)ist6_stack_top;
	tss.ist7 = (uint64_t)ist7_stack_top;
	tss.iopb = 0xFFFF;

	gdt_set_desc(0, 0, 0, 0, 0);
	/* Kernel */
	gdt_set_desc(1, 0, 0xFFFFF, 0x9A, 0xCF);
	gdt_set_desc(2, 0, 0xFFFFF, 0x92, 0xCF);

	/* User */
	gdt_set_desc(3, 0, 0xFFFFF, 0xFA, 0xCF);
	gdt_set_desc(4, 0, 0xFFFFF, 0xF2, 0xCF);
	
	gdt_set_desc(5, (uint32_t)&tss, sizeof(tss) - 1, 0x89, 0);

	gdt_load();
}