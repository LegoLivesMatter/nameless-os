; Code for getting the BIOS E820 memory map

bits 16
section .text

; Return: ECX - number of entries, DI - pointer to map
get_e820_map:
	push eax
	push ebx
	push edx
	push esi
	push es

	xor ax, ax
	mov es, ax
	push dword 1
	sub sp, 20
	xor si, si
	xor ebx, ebx
	mov edx, 0x534d4150
	mov eax, 0xe820
	mov ecx, 24
	mov di, sp
	int 0x15
	jc .no_e820
	cmp eax, 0x534d4150
	jne .unexpected
	inc si
	.loop:
		push dword 1
		sub sp, 20
		mov di, sp
		mov eax, 0xe820
		mov ecx, 24
		int 0x15
		jc .carry_done
		inc si
		cmp ebx, 0
		je .done
		jmp .loop
	.carry_done:
		add sp, 24
	.done:
		movzx ecx, si
		movzx edi, sp
		mov ax, 24
		mul si
		add sp, ax
		pop es
		pop esi
		pop edx
		pop ebx
		pop eax
		ret
	.no_e820:
		print no_e820
		hlt
		jmp $-1
	.unexpected
		print e820_unexpected
		call print_dword
		hlt
		jmp $-1

section .rodata
no_e820: db "BIOS does not support E820, cannot proceed!", 0xd, 0xa, 0
e820_unexpected: db "Unexpected value in EAX after getting map entry: expected SMAP, got ", 0
