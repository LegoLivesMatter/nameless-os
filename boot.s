	bits 16 ; boot sectors run in real mode
	org 7c00h ; boot sector is always loaded at 0x7c00

	mov al, 1 ; update cursor after write
	mov bh, 0 ; write to page 0
	mov bl, 00000111b ; attribute: high nibble is foreground (light gray),
					  ; low nibble is background (black)
					  ; NOTE: in the binary, these will be inverted, so low nibble here will become high nibble in binary and vice versa!
	mov cx, msg1end - msg1 ; size of message
	mov dl, 0 ; column to write to
	mov dh, 0 ; row to write to
	push cs ; push code segment to stack
	pop es ; pop pushed code segment to extra segment, this serves as the base to the string
	mov bp, msg1 ; offset where the string is
	mov ah, 13h ; write string subservice 
	int 10h ; call BIOS interrupt 0x10
	jmp msg1end ; jmp to endless loop
	msg1 db "Gotta start somewhere"
	msg1end: jmp msg1end

	times 510-($-$$) db 0 ; fill with 0 until 0x1fc is reached
	dw 0xaa55 ; BIOS MBR signature
