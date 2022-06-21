bits 32
extern kmain
extern __STACK_BOTTOM__

section .text
global _start
_start:
	; Here, the stack will look like:
	; return address, map pointer, map size.

	; Skip the return address.
	add esp, 4
	; Save the kernel arguments.
	; Can this be done without the global variables?
	pop dword [e820_map]
	pop dword [e820_map_size]
	sub esp, 12
	; Do the stack switch.
	mov ebp, __STACK_BOTTOM__
	mov esp, ebp
	; Restore the kernel arguments.
	push dword [e820_map_size]
	push dword [e820_map]
	; Call the actual kernel.
	call kmain
	hlt
	jmp $-1

; Temporarily save arguments here to survive the upcoming stack switch.
section .bss
e820_map: resd 1
e820_map_size: resd 1
