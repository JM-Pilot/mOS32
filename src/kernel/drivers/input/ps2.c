/* MIT LICENSE (C) 2026 JM-Pilot */
#include "ps2.h"
#include "../../arch/x86/irq.h"
#include "../../arch/x86/asm.h"
#include "../../arch/x86/idt.h"
#include <libk/stdio.h>
#include "../../console/tty.h"
#include <stdbool.h>

char kbdus[128] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
	0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
char kbdus_shift[128] = {
	0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
	0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
	0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


volatile char key_pressed = 0;
bool is_shift = false;
void irq1_handler(struct register_frame *r){
	uint8_t scancode = inb(0x60);
	if (scancode == 0x2A || scancode == 0x36) is_shift = true;
    	if (scancode == 0xAA || scancode == 0xB6) is_shift = false;
	if (scancode & 0x80){
	} else {
		if (is_shift){
			key_pressed = kbdus_shift[scancode];
			return;
		}
		key_pressed = kbdus[scancode];		
	}
}

char ps2_getch(){
	while (key_pressed == 0);
	char c = key_pressed;
	key_pressed = 0;
	return c;
}
void ps2_init(){
	irq_install_handler(1, irq1_handler);
}