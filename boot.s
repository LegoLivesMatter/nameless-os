	bits 16 ; boot sectors run in real mode
	org 7c00h ; boot sector is loaded at 0x7c00

	mov dx, 0DEADh ; number to print
	mov cl, 0 ; column
	mov ch, 0 ; row
	call print_word
	mov dx, 0BEEFh
	mov cl, 0
	mov ch, 1
	call print_word

	jmp $

%include "print.s"

	times 510-($-$$) db 0 ; fill with 0 until 0x1fc is reached
	dw 0AA55h ; BIOS MBR signature
