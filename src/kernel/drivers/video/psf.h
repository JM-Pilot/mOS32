#include <stdint.h>
#ifndef PSF_H
#define PSF_H

#define PSF1_FONT_MAGIC 0x0436
struct PSF1_font{
	uint16_t magic;
	uint8_t font_mode;
	uint8_t char_size;
};

#define MAIN_FONT_WIDTH 8
#define MAIN_FONT_HEIGHT 16
extern struct PSF1_font *font_main;
// Loads Terminus u16n 
void psf1_load();
void psf1_render_char(uint8_t c, int x, int y, uint32_t fg, uint32_t bg);
#endif