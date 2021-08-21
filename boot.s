	bits 16 ; boot sectors run in real mode
	org 7c00h ; boot sector is loaded at 0x7c00

_start
	mov di, printing_numbers
	call print

	or bl, 1
	mov dx, 0DEADh ; number to print
	call print_word
	
	mov di, space
	call print
	
	xor bl, bl
	mov dx, 0BEEFh
	call print_word

	mov di, waiting
	call print

	mov di, keystroke
	mov bl, 1

.loop
	mov ah, 0
	int 16h
	cmp ah, 1Ch
	je .enterpressed
	jmp .continue
	cmp ah, 0Eh
	je .backspace
	jmp .continue
.enterpressed
	pusha
	mov ah, 0Eh
	mov al, 0Dh
	int 10h
	mov al, 0Ah
	int 10h
	popa
	jmp .loop
.backspace
	pusha
	mov ah, 03h
	mov bh, 0
	int 10h
	dec dl
	mov ah, 02h
	int 10h
	mov ah, 0Eh
	mov al, ' '
	int 10h
	mov ah, 02h
	int 10h
	popa
	jmp .loop
.continue
	mov [keystroke], al
	call print
	jmp .loop

printing_numbers db "Printing some random hex numbers", 0
waiting db "I await keystrokes", 0
keystroke db "$", 0
space db " ", 0

%include "print.s"

	times 510-($-$$) db 0 ; fill with 0 until 0x1fc is reached
	dw 0AA55h ; BIOS MBR signature
