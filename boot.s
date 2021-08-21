	bits 16 ; boot sectors run in real mode
	org 7c00h ; boot sector is loaded at 0x7c00

	mov di, printing_numbers
	call print

	mov dx, 0DEADh ; number to print
	call print_word
	mov dx, 0BEEFh
	call print_word

	mov di, goodbye
	call print

	jmp $

printing_numbers: db "Printing some random hex numbers", 0
goodbye: db "Thank you for your attention", 0

%include "print.s"

	times 510-($-$$) db 0 ; fill with 0 until 0x1fc is reached
	dw 0AA55h ; BIOS MBR signature
