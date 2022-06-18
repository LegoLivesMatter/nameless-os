bits 16
section .text

%macro in_wait 0
	push ax
%%loop:
	in al, 0x64
	test al, 2
	jnz %%loop
	pop ax
%endmacro

%macro out_wait 0
	push ax
%%loop:
	in al, 0x64
	test al, 1
	jz %%loop
	pop ax
%endmacro

; CF set = A20 enabled
check_a20:
	push esi
	push edi
	push ax
	mov esi, 0x500
	mov edi, 0x100500

	mov al, [esi]
	mov ah, [edi]

	mov [esi], byte 0
	mov [edi], byte 0xff
	cmp [esi], byte 0xff
	
	mov [esi], al
	mov [edi], ah

	pop ax
	pop edi
	pop esi
	ret

enable_a20:
	push ax
	mov ax, 0x2401
	int 0x15
	jc .i8042_method
	call check_a20
	jnc .i8042_method
	pop ax
	ret
	.i8042_method:
		in_wait
		mov al, 0xad
		out 0x64, al
		in_wait
		mov al, 0xd0
		out 0x64, al
		out_wait
		in al, 0x60
		or al, 2
		push ax
		in_wait
		mov al, 0xd1
		out 0x64, al
		in_wait
		pop ax
		out 0x60, al
		in_wait
		mov al, 0xae
		out 0x60, al
		call check_a20
		pop ax
		ret

