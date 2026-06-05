/* MIT LICENSE (C) 2026 JM-Pilot */
.code32

.extern gdtr
.global gdt_load
gdt_load:
	lgdt gdtr
	mov $0x28, %ax
	ltr %ax

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	
	ljmp $0x08, $.gdt_reload_cs
.gdt_reload_cs:
	ret
	