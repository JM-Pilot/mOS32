/* MIT LICENSE (C) 2026 JM-Pilot */
#include "timer.h"
#include "../arch/x86/irq.h"
#include "../arch/x86/asm.h"
#include "../arch/x86/idt.h"
#include <libk/stdio.h>
void timer_phase(int hz){
	int divisor = 1193180 / hz;
	outb(0x43, 0x36);
	outb(0x40, divisor & 0xFF);
	outb(0x40, divisor >> 8);
}

int ticks = 0;

void irq0_handler(struct register_frame *r){
	ticks++;
}

int timer_get_ticks(){
	return ticks;
}

void timer_init(){
	timer_phase(100);
	irq_install_handler(0, irq0_handler);
}