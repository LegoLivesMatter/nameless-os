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

	call switch_to_pm

halt
	jmp $

switch_to_pm
	mov bx, 0B800h
	mov es, bx ; set extra segment to starting address of video RAM
	mov byte [es:0], 'L' ; print an L to screen, to let us know that we actually got here

	cli ; disable interrupts
	xor ax, ax ; clear accumulator
	mov ds, ax ; clear data segment, this makes sure the next instruction reads from the right place
	lgdt [gdt_desc] ; load the Global Descriptor Table
	
	mov eax, cr0 ; move Control Register 0 to accumulator
	or eax, 1 ; flip the bit which controls memory protection
	mov cr0, eax ; move the accumulator back to CR0
	jmp CODE_SEG:protected ; not quite there yet, need to do a far jump to clean the pipeline from any 16-bit instructions
						   ; note that the jump does not have to be physically far away, it just needs to use a segmented address

	bits 32 ; we are finally in 32-bit mode
protected
	mov ax, DATA_SEG ; put the selector for the data segment in the accumulator
	; in real mode, segmentation works by taking one of these segment registers, shifting it right by 4 bits or 1 hex digit
	; and then adding a 16-bit offset to form a 20-bit address
	; example: 1234h:5678h
	; 1234h is shifted to 12340h, 12340h + 5678h is 179B8h
	; in 32-bit protected mode, these segment registers do not hold the segment address itself, but a selector in the GDT
	; so we have to update the segment registers accordingly
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	; reinitialize the stack at a safe location
	mov ebp, 090000h
	mov esp, ebp

	; print a string to let us know that we survived the switch
	mov ebx, pm_success
	call pmprint

	; transfer control to the kernel
	call KERNEL_OFFSET

	; above call should not return in normal circumstances, but if it does hang forever
	jmp $

pmprint
	pusha ; save registers to stack
	mov edx, video_memory ; initialize dx with location of VRAM

.loop
	mov al, [ebx] ; read next char and put it in al
	mov ah, 0Fh ; puts the VGA text mode color white on black into ah

	cmp al, 0 ; if the next character is null, we reached end of string
	je .done ; so return the instruction

	mov [edx], al ; otherwise put the next character in the video memory
	mov [edx+1], ah ; do the same for its color

	inc ebx ; point to next character in string
	add edx, 2 ; point to next character in VRAM

	jmp .loop ; go back to the loop

.done
	popa ; restore registers from stack
	ret ; return

pm_success db "Now in protected mode", 0
video_memory equ 0B8000h

; the actual Global Descriptor Table
; refer to Volume 3, Chapter 2, 2.1.1 of Intel's 64 and IA-32 Software Developer's Manual for more info
gdt_start
null_seg
	dq 0
code_seg
	dw 0FFFFh
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0
data_seg
	dw 0FFFFh
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0
gdt_end
gdt_desc
	dw gdt_end - gdt_start - 1
	dd gdt_start
CODE_SEG equ code_seg - gdt_start
DATA_SEG equ data_seg - gdt_start

BOOT_DRIVE db 0 ; reserve a spot in RAM for our boot drive variable

	times 510-($-$$) db 0
	dw 0AA55h ; MBR signature
