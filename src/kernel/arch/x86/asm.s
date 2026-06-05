.code32
.global asm_restore_registers
asm_restore_registers:
	mov 4(%esp), %eax
	
	mov 4(%eax), %ebx
	mov 8(%eax), %ecx
	mov 12(%eax), %edx
	mov 16(%eax), %edi
	mov 20(%eax), %esi
	mov 24(%eax), %ebp
	mov 28(%eax), %esp
	
	jmp *32(%eax)