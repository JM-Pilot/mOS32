/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#ifndef TTY_H
#define TTY_H

#define STDOUT 0xAB
#define STDIN 0xAC

#define STDIN_BUF_SIZE 1024
struct tty {
	int cursor_x;
	int cursor_y;
	uint32_t fg;
	uint32_t bg;
};

extern struct tty tty_main;
extern struct tty *tty_current;

void tty_init(struct tty *t, uint32_t fg, uint32_t bg);

void tty_set_current(struct tty *t);

void tty_write_char(char c);

int tty_read(char *buf, int n);

#endif