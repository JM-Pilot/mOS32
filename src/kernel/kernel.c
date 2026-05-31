#include <boot/multiboot.h>
#include "drivers/video/framebuffer.h"
#include "drivers/video/psf.h"
void kernel_main(multiboot_info_t *mb_info){
	fb_init(mb_info);
	psf1_load();
	for (int i = 0; i < 100; i++)
		fb_plot_pixel(0xFFFFFF, i, i);
	psf1_render_char('C', 10, 10, 0xFFFFFF, 0);
	for (;;);
}

void hcf(){
	asm volatile ("cli");
	for (;;)
		asm volatile ("hlt");
}