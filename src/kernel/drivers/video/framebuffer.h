/* MIT LICENSE (C) 2026 JM-Pilot */
#include <boot/multiboot.h>
#include <stdint.h>
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/* We only need the important parts */
struct framebuffer {
	uint32_t *addr;
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint8_t bpp;
};

extern struct framebuffer fb_main;
void fb_init(multiboot_info_t *mb_info);
void fb_plot_pixel(uint32_t color, int x, int y);
void fb_clr(uint32_t col);
#endif