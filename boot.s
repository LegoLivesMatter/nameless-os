	bits 16 ; boot sectors run in real mode
	org 7c00h ; boot sector is loaded at 0x7c00

_start
	mov [boot_drive], dl ; BIOS sets dl to number of boot drive, we don't want to forget this

	mov bp, 8000h ; Move the stack away
	mov sp, bp

	push cs
	pop es
	mov bx, 9000h ; Load at 0000:9000
	mov dh, 2 ; Load 5 sectors
	mov dl, [boot_drive] ; Load from boot drive
	call read_sectors

	mov dx, [9000h] ; Print value at 9000h, should be 0xDEAD
	call print_word 
	mov dx, [9000h + 512] ; Print value at 9200h, should be 0xBEEF
	call print_word

	jmp $ ; Endless loop

%include "print.s"
%include "disk.s"

boot_drive resb 1 ; Reserve 1 byte for boot drive number

times 510-($-$$) db 0 ; fill with 0 until 0x1fc is reached
dw 0AA55h ; BIOS MBR signature

times 256 dw 0DEADh ; Fill in 2nd sector with 0xDEAD
times 256 dw 0BEEFh ; Fill in 3rd sector with 0xBEEF
