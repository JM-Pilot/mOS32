#include <stdint.h>
#include "../../arch/x86/asm.h"

void ata_read(uint32_t lba, uint8_t count, uint8_t slave, uint16_t *buf){
	outb(0x1F6, 0xE0 | (slave << 4) | ((lba >> 24) & 0x0F));
	outb(0x1F1, 0x00);
	outb(0x1F2, count);
	outb(0x1F3, (uint8_t)lba);
	outb(0x1F4, (uint8_t)(lba >> 8));
	outb(0x1F5, (uint8_t)(lba >> 16));
	outb(0x1F7, 0x20);

	for (int i = 0; i < count; i++){
		while (inb(0x1F7) & 0x80);
		for (int j = 0; j < 256; j++){
			buf[i * 256 + j] = inw(0x1F0);
		}
	}
}

/* Same thing as ata_read with minimal changes wow */

void ata_write(uint32_t lba, uint8_t count, uint8_t slave, uint16_t *buf){
	outb(0x1F6, 0xE0 | (slave << 4) | ((lba >> 24) & 0x0F));
	outb(0x1F1, 0x00);
	outb(0x1F2, count);
	outb(0x1F3, (uint8_t)lba);
	outb(0x1F4, (uint8_t)(lba >> 8));
	outb(0x1F5, (uint8_t)(lba >> 16));
	outb(0x1F7, 0x30);
	for (int i = 0; i < count; i++){
		while (inb(0x1F7) & 0x80);
		for (int j = 0; j < 256; j++){
			outw(0x1F0, buf[i * 256 + j]);
		}
	}
	outb(0x1F7, 0xE7);
	while (inb(0x1F7) & 0x80);
}