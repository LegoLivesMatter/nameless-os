; Routines for printing

bits 16
section .text

print_str:
	push ax
	push bx
	push si
	mov ah, 0xe
	xor bh, bh
	mov bl, 0xf
.loop:
	lodsb
	cmp al, 0
	je .done
	int 0x10
	jmp .loop
.done:
	pop si
	pop bx
	pop ax
	ret

; AL - nibble to print
; CF - clear prints low nibble, set prints high nibble
print_nibble:
	print hex_delm
.no_delm:
	pushf
	push ax
	push bx
	pushf
	xor bh, bh
	mov bl, 0xf
	mov ah, 0xe
	popf
	jnc .no_shift
	shr al, 4
.no_shift:
	and al, 0xf
	cmp al, 0xa
	jge .letter
	.numeric:
		add al, '0'
		int 0x10
		jmp .done
	.letter:
		add al, 'a'-10
		int 0x10
.done:
	pop bx
	pop ax
	popf
	ret

; AL - byte to print
print_byte:
	print hex_delm
.no_delm:
	pushf
	stc
	call print_nibble.no_delm
	clc
	call print_nibble.no_delm
	popf
	ret
	
print_word:
	print hex_delm
.no_delm:
	ror ax, 8
	call print_byte.no_delm
	ror ax, 8
	call print_byte.no_delm
	ret

print_dword:
	print hex_delm
.no_delm:
	ror eax, 16
	call print_word.no_delm
	ror eax, 16
	call print_word.no_delm
	ret

; Do a dump of all general purpose regs
critical_error:
	print crit_err
	print eax_s
	call print_dword
	print space
	print ebx_s
	mov eax, ebx
	call print_dword
	print space
	print ecx_s
	mov eax, ecx
	call print_dword
	print space
	print edx_s
	mov eax, edx
	call print_dword
	print newline
	print esi_s
	mov eax, esi
	call print_dword
	print space
	print edi_s
	mov eax, edi
	call print_dword
	print newline
	print cs_s
	mov ax, cs
	call print_word
	print space
	print ds_s
	mov ax, ds
	call print_word
	print space
	print es_s
	mov ax, es
	call print_word
	print space
	print ss_s
	mov ax, ss
	call print_word
	hlt
	jmp $-1
