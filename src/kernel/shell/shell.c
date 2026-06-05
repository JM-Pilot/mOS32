/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#include "shell.h"
#include "../console/tty.h"
#include <libk/stdio.h>
#include <libk/string.h>
#include "../drivers/video/framebuffer.h"
#include "../mm/pmm.h"
#include "../fs/fat32.h"
#include <libk/stdlib.h>
char buf[1024];


void ls_root(){
	struct fat32_dir_entry *dir = fat32_read_dir(
		fat32_cluster_to_lba(fat_info.root_clust));
	for (int i = 0; i < 512 / 32; i++) {
		if (dir[i].filename[0] == 0x00) break;
		if (dir[i].filename[0] == 0xE5) continue;
		for (int j = 0; j < 11; j++)
			k_printf("%c", dir[i].filename[j]);
		k_printf("\n");
	}
	kfree(dir);
}

void shell_execute(){
	if (k_strncmp(buf, "help", 4) == 0){
		k_printf("mOS shell, made by JM-Pilot\n");
		k_printf("help    -- Give information and list all cmds\n");
		k_printf("sysinfo -- Give information about the system\n");
		k_printf("clear   -- Clears the screen\n");
		k_printf("ls	  -- list all files in root (/)\n");
	}
	else if (k_strncmp(buf, "clear", 5) == 0){
		tty_clr_and_reset(&tty_main);
	}
	else if (k_strncmp(buf, "sysinfo", 8) == 0){
		k_printf("myOS32\n");
		k_printf("LICENSE: MIT LICENSE\n");
		k_printf("CREATOR: JM-PILOT\n");
		k_printf("MODE: KERNEL\n");
		k_printf("MEMORY: %uMB\n", get_total_mem() / (1024 * 1024));
		k_printf("SHELL: kShell\n");
		k_printf("VERSION: 0.2\n");
	}
	else if (k_strncmp(buf, "ls", 2) == 0){
		ls_root();
	} 
	else {
		k_printf("INVALID CMD %s\n", buf);
		k_printf("TYPE 'help' TO LIST CMDS\n");
	}
}

void shell_prompt(){
	k_memset(buf, 0, sizeof(buf));
	k_puts("KERNEL > ", STDOUT);
	k_gets(buf, sizeof(buf));
	k_putc('\n', STDOUT);
}

void shell_init(){
	tty_clr_and_reset(&tty_main);
	k_puts("Welcome to mOS32 Shell!\n", STDOUT);
}