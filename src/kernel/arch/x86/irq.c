/* MIT LICENSE (C) 2026 JM-Pilot */
#include "asm.h"
#include <stdint.h>
#include "idt.h"
#include "irq.h"

void *irq_routines[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void irq_install_handler(int irq, void (*handler)(struct register_frame *r)){
	irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq){
	irq_routines[irq] = 0;
}

void irq_handler(struct register_frame *reg){
	void (*handler)(struct register_frame *r) = irq_routines[
						reg->int_no - 32];
						/* Must keep 80 char limit */
	if (handler) 
		handler(reg);
	if (reg->int_no >= 40)
		outb(0xA0, 0x20);
	outb(0x20, 0x20);
}


