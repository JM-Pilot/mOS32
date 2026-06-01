#include <boot/multiboot.h>
#include "drivers/video/framebuffer.h"
#include "drivers/video/psf.h"
#include <kernel/kernel.h>
#include "kernel.h"
#include "console/tty.h"
#include <libk/stdio.h>
void kernel_main(multiboot_info_t *mb_info){
	fb_init(mb_info);
	psf1_load();
	psf1_render_char('C', 10, 10, 0xFFFFFF, 0);
	tty_init(&tty_main, 0xFFFFFF, 0);
	tty_set_current(&tty_main);
	k_puts("HELLO WORLD!\n", STDOUT);
	for (;;);
}

void hcf(){
	asm volatile ("cli");
	for (;;)
		asm volatile ("hlt");
}