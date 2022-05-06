; Routine for enabling unreal mode, which allows using 32-bit offsets in real mode

enable_unreal:
	cli
	push eax
	push bx
	push ds

	lgdt [gdt_info]
	mov eax, cr0
	or al, 1
	mov cr0, eax

	jmp $+2

	mov bx, 0x8
	mov ds, bx

	and al, 0xfe
	mov cr0, eax

	pop ds
	pop bx
	pop eax
	sti
	ret

; GDT with 1 flat data segment descriptor
gdt_info:
	dw gdt_end-gdt_start-1
	dd gdt_start
gdt_start: dq 0 ; null entry
gdt_flat:
	dw 0xffff
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0
gdt_end:
