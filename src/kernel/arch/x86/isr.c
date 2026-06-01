/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#include "idt.h"
#include <libk/stdio.h>
#include "irq.h"
#include "../../drivers/video/framebuffer.h"
#include "../../console/tty.h"
#include "../../kernel.h"


const char *exception_messages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
};

void isr_exception_handler(struct register_frame *frame){
	if (frame->int_no > 31 && frame->int_no < 48){
		irq_handler(frame);
	}
	if (frame->int_no < 32){
		fb_clr(0x577AB3);
		tty_init(&tty_main, 0xFFFFFF, 0x577AB3);
		tty_set_current(&tty_main);
		k_printf("INTERRUPT REACHED!\nINTERRUPT %s\n", 
			(frame->int_no < 19 ? exception_messages[frame->int_no] 
				: "reserved"));
		
		k_printf("REBOOT NOW!\n");
		k_printf("EAX: %x, ECX: %x, EDX: %x, EBX: %x\n", 
			frame->eax, frame->ecx, frame->edx, frame->ebx);
		k_printf("ESP: %x, EBP: %x, ESI: %x, EDI: %x\n",
			frame->esp, frame->ebp, frame->esi, frame->edi);
		k_printf("GS: %x, FS: %x, ES: %x, DS: %x\n",
			frame->gs, frame->fs, frame->es, frame->ds);
		k_printf("EIP: %x, CS %x", frame->eip, frame->cs);
		hcf();
	}
}