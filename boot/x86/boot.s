	bits 16 ; boot sectors run in real mode
	org 7C00h ; BIOS loads us at 0x7c00

KERNEL_OFFSET equ 1000h ; where we will load our kernel

	mov [BOOT_DRIVE], dl ; BIOS puts the number of our boot drive in dl, so we will want to remember this

	mov bp, 9000h ; initialize stack at a spot sufficiently distanced from this code
	mov sp, bp

	mov di, 0

	xor ax, ax ; clear accumulator
	int 13h ; BIOS disk services, with a cleared accumulator this will reset the disk controller
	jc reset_error ; halt if controller reset fails

	mov ah, 2 ; instruct BIOS's interrupt 13h to read sectors
	mov al, 10 ; load 10 sectors, might be excessive (for now) but it's still good to do so
	xor ch, ch ; read from 1st cylinder
	mov cl, 2 ; start reading from 2nd sector, right after the boot sector
	xor dh, dh ; read from 1st head
	xor bx, bx ; clear B register
	mov es, bx ; clear extended segment
	mov bx, KERNEL_OFFSET ; put the sectors in our desired offset
	; dl holds the number of the drive to read from, but BIOS already filled this in
	int 13h ; do the read
	jc read_error ; halt if read fails
	cmp al, 10 ; make sure we actually read 10 sectors
	jl read_error ; halt if we didn't
	jmp switch_to_pm ; if all is good, begin the switch to 32-bit protected mode

reset_error
	mov bx, 0B800h
	mov es, bx
	mov byte [es:di], '1'
	jmp halt

read_error
	mov bx, 0B800h
	mov es, bx
	mov byte [es:di], '2'
	jmp halt

halt
	jmp $

%include "a20.s"
%include "protected.s"

BOOT_DRIVE db 0 ; reserve a spot in RAM for our boot drive variable

	times 510-($-$$) db 0
	dw 0AA55h ; MBR signature
