/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#ifndef GDT_H
#define GDT_H

struct gdt_r {
	uint16_t size;
	uint32_t offset;
}__attribute__((packed));

struct gdt_desc {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_mid;
	uint8_t  access_byte;
	uint8_t  flags;
	uint8_t  base_high;
}__attribute__((packed));

void gdt_init();
#endif