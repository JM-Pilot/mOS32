/* MIT LICENSE (C) 2026 JM-Pilot */
#include <libk/stdio.h>
#include <stddef.h>
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

char k_getc(){
	char c[1];
	tty_read(c, 1);
	return c[0];
}
void k_gets(char *buf, size_t size){
	tty_read(buf, size);
}