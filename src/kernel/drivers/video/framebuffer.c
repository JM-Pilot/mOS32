#include <boot/multiboot.h>
#include <stdint.h>
#include "framebuffer.h"

struct framebuffer fb_main;
void fb_init(multiboot_info_t *mb_info){
	fb_main.addr = (uint32_t*)(uintptr_t)mb_info->framebuffer_addr;
	fb_main.width = mb_info->framebuffer_width;
	fb_main.height = mb_info->framebuffer_height;
	fb_main.pitch = mb_info->framebuffer_pitch;
	fb_main.bpp = mb_info->framebuffer_bpp;
}

void fb_plot_pixel(uint32_t color, int x, int y){
	fb_main.addr[y * (fb_main.pitch / 4) + x] = color;
}

void fb_clr(uint32_t col){
	for (uint32_t i = 0; i < fb_main.height; i++)
		for (uint32_t j = 0; j < fb_main.width; j++)
			fb_plot_pixel(col, j, i);	
}