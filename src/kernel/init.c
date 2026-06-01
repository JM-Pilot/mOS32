/* MIT LICENSE (C) 2026 JM-Pilot */
#include "kernel.h"
#include <stdint.h>
#include <boot/multiboot.h>
#include "drivers/video/framebuffer.h"
#include "drivers/video/psf.h"
#include <kernel/kernel.h>
#include "kernel.h"
#include "console/tty.h"
#include <libk/stdio.h>
#include "arch/x86/gdt.h"
#include "arch/x86/idt.h"
#include "arch/x86/pic.h"
#include "drivers/timer.h"
#include "drivers/input/ps2.h"
void kernel_init(multiboot_info_t *mb_info){
	asm volatile ("cli");
	fb_init(mb_info);
	psf1_load();
	tty_init(&tty_main, 0xFFFFFF, 0);
	tty_set_current(&tty_main);
	k_puts("OUTPUT & TTY Initialized\n", STDOUT);
	gdt_init();
	k_puts("GDT Initialized\n", STDOUT);
	idt_init();
	k_puts("IDT Initialized\n", STDOUT);
	pic_init();
	k_puts("PIC Initialized\n", STDOUT);
	timer_init();
	k_puts("TIMER (irq0) Initialized\n", STDOUT);
	ps2_init();
	k_puts("KBD (PS2, irq1) Initialized\n", STDOUT);
	asm volatile ("sti");

	k_puts("Welcome to mOS32, you will be presented into a kernel shell\n",
	 STDOUT);
}