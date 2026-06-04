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
#include "mm/pmm.h"
#include "mm/paging.h"
#include "mm/heap.h"
#include "fs/fat32.h"
#include "drivers/disk/ata.h"
#include <libk/stdlib.h>
#include <libk/string.h>
#include "shell/shell.h"
heap_bm_t k_heap;

void kernel_init(multiboot_info_t *mb_info){
	asm volatile ("cli");
	fb_init(mb_info);
	psf1_load();
	tty_init(&tty_main, 0xFFFFFF, 0);
	tty_set_current(&tty_main);
	k_puts("[SYSIO]		OUTPUT & TTY Initialized\n", STDOUT);
	gdt_init();
	k_puts("[SYS] 		GDT Initialized\n", STDOUT);
	idt_init();
	k_puts("[SYS] 		IDT Initialized\n", STDOUT);
	pic_init();
	k_puts("[SYS] 		PIC Initialized\n", STDOUT);
	timer_init();
	k_puts("[SYS] 		TIMER (irq0) Initialized\n", STDOUT);
	ps2_init();
	k_puts("[SYSIO] 	KBD (PS2, irq1) Initialized\n", STDOUT);
	pmm_init(mb_info);
	k_puts("[SYS] 		PMM Initialized\n", STDOUT);
	paging_init((uint32_t)fb_main.addr);
	k_puts("[SYS] 		PAGING Initialized\n", STDOUT);

	heap_init(&k_heap);
	for (int i = 0; i < 512; i++) {
		uint32_t heap_phys = pmm_alloc();
		vmm_map_page((void*)heap_phys, (void*)(0x500000 + i*4096), 3);
		heap_add_block(&k_heap, 0x500000 + i*4096, 4096, 16);
	}
	k_puts("[SYS] 		HEAP Initialized\n", STDOUT);

	fat32_init();
	k_puts("[SYSFS] 	FAT32 Initialized\n", STDOUT);

	asm volatile ("sti");

	k_puts("Welcome to mOS32, you will be presented into a kernel shell\n",
	STDOUT);
	shell_init();
}