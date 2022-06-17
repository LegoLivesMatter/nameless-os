bits 32
extern kmain
extern __STACK_BOTTOM__

global _start
_start:
	mov ebp, __STACK_BOTTOM__
	mov esp, ebp
	push ecx
	push edi
	call kmain
	hlt
	jmp $-1
