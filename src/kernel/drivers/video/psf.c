/* MIT LICENSE (C) 2026 JM-Pilot */
#include "psf.h"
#include "framebuffer.h"
#include <stdint.h>
#include "../../kernel.h"

extern char _binary_src_kernel_misc_ter_u16n_psf_start;
struct PSF1_font *font_main;

void psf1_load(){
	font_main = (struct PSF1_font*)&
	_binary_src_kernel_misc_ter_u16n_psf_start;

	if (font_main->magic != PSF1_FONT_MAGIC)
		hcf();
}
void psf1_render_char(uint8_t c, int x, int y, uint32_t fg, uint32_t bg){
	uint8_t bpg = ((MAIN_FONT_WIDTH + 7) / 8) * MAIN_FONT_HEIGHT;
	uint8_t *glyph = (uint8_t*)&
	_binary_src_kernel_misc_ter_u16n_psf_start + 4 +
	c * bpg;

	for (uint8_t gy = 0; gy < MAIN_FONT_HEIGHT; gy++){
		uint8_t byte = glyph[gy];
		for (uint8_t gx = 0; gx < MAIN_FONT_WIDTH; gx++){
			if (byte & (0x80 >> gx))
				fb_plot_pixel(fg, x + gx, y + gy);
			else
				fb_plot_pixel(bg, x + gx, y + gy);
		}
	}
}