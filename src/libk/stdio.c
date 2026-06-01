/* MIT LICENSE (C) 2026 JM-Pilot */
#include <libk/stdio.h>
#include "../kernel/console/tty.h"
void k_putc(char c, int fd){
	if (fd == STDOUT){
		tty_write_char(c);
	}
}
void k_puts(const char *s, int fd){
	for (int i = 0; s[i] != '\0'; i++){
		k_putc(s[i], fd);
	}
}