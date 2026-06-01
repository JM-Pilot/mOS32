/* MIT LICENSE (C) 2026 JM-Pilot */
#include <boot/multiboot.h>
#include "kernel.h"
#include <libk/stdio.h>
#include <libk/string.h>
#include "console/tty.h"
#include "shell/shell.h"
void kernel_main(multiboot_info_t *mb_info){
	kernel_init(mb_info);
	
	while (1){
		shell_prompt();
		shell_execute();
	}
}

void hcf(){
	asm volatile ("cli");
	for (;;)
		asm volatile ("hlt");
}