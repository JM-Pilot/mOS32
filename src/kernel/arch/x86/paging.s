.code32

.section .text
.global load_page_dir
.global enable_paging

load_page_dir:
	push %ebp
	mov %esp, %ebp
	mov 8(%esp), %eax
	mov %eax, %cr3
	mov %ebp, %esp 
	pop %ebp
	ret
enable_paging:
	push %ebp
	mov %esp, %ebp
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0
	mov %ebp, %esp
	pop %ebp
	ret