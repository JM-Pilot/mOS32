/* MIT LICENSE (C) 2026 JM-Pilot */
#include "stdint.h"
#include <stddef.h>
#include "tty.h"
#include "../drivers/video/psf.h"
#include "../drivers/video/framebuffer.h"
#include "../drivers/input/ps2.h"
#include <libk/stdio.h>
#include <libk/string.h>
struct tty tty_main;
struct tty *tty_current;

void tty_init(struct tty *t, uint32_t fg, uint32_t bg) {
	t->cursor_x = 0;
	t->cursor_y = 0;
	t->fg = fg;
	t->bg = bg;

	fb_clr(bg);
}
void tty_set_current(struct tty *t){
	tty_current = t;
}

#define MAX_CELLS_X (fb_main.width / MAIN_FONT_WIDTH)
#define MAX_CELLS_Y (fb_main.height / MAIN_FONT_HEIGHT)
void tty_write_char(char c){
	switch (c){
		case '\n':
			tty_current->cursor_x = 0;
			tty_current->cursor_y++;
			if ((uint64_t)tty_current->cursor_y >= MAX_CELLS_Y){
				tty_scroll_up();
			}
			return;
		case '\r':
			tty_current->cursor_x = 0;
			return;
		case '\b':
			if (tty_current->cursor_x == 0) return;
			tty_current->cursor_x--;
			tty_write_char(' ');
			tty_current->cursor_x--;
			return;
		case '\t':
			if ((uint64_t)tty_current->cursor_x + 4 >= MAX_CELLS_X){
				tty_write_char('\n');
				return;
			}
			tty_current->cursor_x += 4;
			return; 
	}
	int x = tty_current->cursor_x * MAIN_FONT_WIDTH;
	int y = tty_current->cursor_y * MAIN_FONT_HEIGHT;

	psf1_render_char(c, x, y, 
		tty_current->fg, tty_current->bg);
	
	if ((uint64_t)(++tty_current->cursor_x) >= MAX_CELLS_X){
		tty_current->cursor_y++;
		tty_current->cursor_x = 0;
		if ((uint64_t)tty_current->cursor_y >= MAX_CELLS_Y){
			tty_scroll_up();
		}
	}
}

int tty_read(char *buf, int n){
	int i = 0;
	char c;
	while (i < n && (c = ps2_getch()) != '\n'){
		if (c == '\b'){
			if (i > 0){
				i--;
				k_putc('\b', STDOUT);
			} 
			continue;
		}
		buf[i++] = c;
		k_putc(c, STDOUT);
	}
	buf[i] = '\0';
	return i;
}

void tty_scroll_up(){
	uint8_t *addr = (uint8_t*)fb_main.addr;
	uint32_t line = fb_main.pitch * MAIN_FONT_HEIGHT;
	k_memmove(addr, addr + line, fb_main.pitch * fb_main.height - line);
	k_memset(addr + fb_main.pitch * fb_main.height - line, 0, line);
	tty_current->cursor_y -= 1;
}

void tty_clr_and_reset(struct tty *t){
	fb_clr(tty_current->bg);
	tty_init(t, tty_current->fg, tty_current->bg);
	tty_set_current(t);
}