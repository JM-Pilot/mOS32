/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#include "shell.h"
#include "../console/tty.h"
#include <libk/stdio.h>
#include <libk/string.h>
#include "../drivers/video/framebuffer.h"
char buf[1024];

void shell_execute(){
	if (k_strncmp(buf, "help", 4) == 0){
		k_printf("mOS shell, made by JM-Pilot\n");
		k_printf("help -- Give information and list all cmds\n");
		k_printf("clear -- Clears the screen\n");
	}
	else if (k_strncmp(buf, "clear", 5) == 0){
		fb_clr(tty_current->bg);
		tty_init(&tty_main, tty_current->fg, tty_current->bg);
		tty_set_current(&tty_main);
	}
	else {
		k_printf("INVALID CMD %s\n", buf);
		k_printf("TYPE 'help' TO LIST CMDS\n");
	}
}
void shell_prompt(){
	k_memset(buf, 0, 1024);
	k_puts("KERNEL > ", STDOUT);
	tty_read(buf, 1023);
	k_putc('\n', STDOUT);
}