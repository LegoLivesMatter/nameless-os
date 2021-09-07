	bits 16
	org 7C00h

KERNEL_OFFSET equ 1000h

	mov [BOOT_DRIVE], dl

	mov bp, 9000h
	mov sp, bp

	mov di, 0

	xor ax, ax
	int 13h
	jc reset_error

	mov ah, 2
	mov al, 10
	xor ch, ch
	mov cl, 2
	xor dh, dh
	xor bx, bx
	mov es, bx
	mov bx, KERNEL_OFFSET
	int 13h
	jc read_error
	cmp al, 10
	jl read_error
	jmp switch_to_pm

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
	mov es, bx
	mov byte [es:0], 'L'

	cli
	xor ax, ax
	mov ds, ax
	lgdt [gdt_desc]
	
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp CODE_SEG:protected

	bits 32
protected
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 090000h
	mov esp, ebp

	mov ebx, pm_success
	call pmprint

	call KERNEL_OFFSET

	jmp $

pmprint
	pusha
	mov edx, video_memory

.loop
	mov al, [ebx]
	mov ah, 0Fh

	cmp al, 0
	je .done

	mov [edx], al
	mov [edx+1], ah

	inc ebx
	add edx, 2

	jmp .loop

.done
	popa
	ret

pm_success db "Now in protected mode", 0
video_memory equ 0B8000h

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

BOOT_DRIVE db 0

	times 510-($-$$) db 0
	dw 0AA55h
