/* MIT LICENSE (C) 2026 JM-Pilot */
	.set ALIGN,    1<<0
	.set MEMINFO,  1<<1 
	.set VIDEO,    1<<2
	.set FLAGS,    ALIGN | MEMINFO | VIDEO
	.set MAGIC,    0x1BADB002
	.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
	.align 4
	.long MAGIC
	.long FLAGS
	.long CHECKSUM
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	/* VIDEO FRAMEBUFFER */
	.long 0
	.long 1024
	.long 768
	.long 32

.global kernel_stack_top
.global ist1_stack_top
.global ist2_stack_top
.global ist3_stack_top
.global ist4_stack_top
.global ist5_stack_top
.global ist6_stack_top
.global ist7_stack_top

.section .bss
	.align 16
kernel_stack_bottom:
	.skip 16384 
kernel_stack_top:
	.align 16
ist1_stack_bottom:
	.skip 4096
ist1_stack_top:
	.align 16
ist2_stack_bottom:
	.skip 4096
ist2_stack_top:
	.align 16
ist3_stack_bottom:
	.skip 4096
ist3_stack_top:
	.align 16
ist4_stack_bottom:
	.skip 4096
ist4_stack_top:
	.align 16
ist5_stack_bottom:
	.skip 4096
ist5_stack_top:
	.align 16
ist6_stack_bottom:
	.skip 4096
ist6_stack_top:
	.align 16
ist7_stack_bottom:
	.skip 4096
ist7_stack_top:

.section .text
	.global _start
	.type _start, @function
_start:
	mov $kernel_stack_top, %esp
	push %ebx
	call kernel_main

.size _start, . - _start
