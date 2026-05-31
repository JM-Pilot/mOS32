#include <boot/multiboot.h>
#include "drivers/video/framebuffer.h"
#include "drivers/video/psf.h"
void kernel_main(multiboot_info_t *mb_info){
	fb_init(mb_info);
	psf1_load();
	psf1_render_char('C', 10, 10, 0xFFFFFF, 0);
	for (;;);
}

void hcf(){
	asm volatile ("cli");
	for (;;)
		asm volatile ("hlt");
}