; x86 bootloader for nameless-os, MBR portion

bits 16
cpu 686
org 0x600

_start:
	cli ; we really don't want to be interrupted during relocation
	; set up segment registers
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0xfe00 ; just under the soon-to-be-loaded VBR, should be more than sufficient space

	; perform self-relocation
	cld
	mov si, 0x7c00
	mov di, 0x600
	mov cx, 0x100 ; 256 words = 512 bytes
	rep movsw
	; some BIOSes may set CS to 0x7c0, work around that
	jmp 0:real_start
real_start:
	sti
	; check for int 13h ext
	call check_int13_ext
	mov byte [BOOT_DRIVE], dl
	; look for active partition
	mov si, part_1
	mov cx, 4
	.check_part_loop:
		mov al, [si]
		test al, 0x80
		jnz .part_found
		add si, 16
		loop .check_part_loop
		.error:
			mov si, no_os
			call print
			hlt
			jmp short $-1
	.part_found:
		; look for any other active partitions, if they exist the partition table is invalid
		cmp cx, 0
		je .load_vbr
		push si 
	.look_other_loop:
		add si, 16
		mov al, [si]
		test al, 0x80
		jnz .invalid_mbr
		loop .look_other_loop
	.load_vbr:
		; load active partition's VBR
		pop si
		mov ax, 0xfe0
		add si, 8
		mov edx, [si]
		xor bx, bx
		call read_sector
		; check is the VBR bootable (ends with 0x55 0xaa), if not halt
		cmp word [0xfffe], 0xaa55
		jne .not_bootable
		mov dl, [BOOT_DRIVE]
		call 0xfe00
		.not_bootable:
			mov si, no_os
			call print
			hlt
			jmp short $-1
	.invalid_mbr:
		mov si, invalid_mbr
		call print
		hlt
		jmp short $-1
check_int13_ext:
	pusha
	mov ah, 0x41
	mov bx, 0x55aa
	mov dl, 0x80
	int 0x13
	jc .no_ext
	test cx, 1
	jz .no_ext
	popa
	ret
	.no_ext:
		mov si, no_int13_ext
		call print
		hlt
		jmp short $-1

; ax = segment
; bx = offset
; edx = start LBA of where to read
read_sector:
	pusha
	; set up temporary DAP
	push dword 0
	push edx
	push ax
	push bx
	push word 1
	push word 0x10
	mov ah, 0x42
	mov dl, byte [BOOT_DRIVE]
	mov si, sp
	int 0x13
	jc .error
	add sp, 16 ; dump our temporary DAP
	popa
	ret
	.error:
		mov si, read_fail
		call print
		hlt
		jmp short $-1

; DS:SI = string
print:
	pusha
	mov ah, 0xe
	xor bh, bh
.loop:
	lodsb
	cmp al, 0
	je .done
	int 10h
	jmp .loop
.done:
	mov al, 0xd
	int 10h
	mov al, 0xa
	int 10h
	popa
	ret

BOOT_DRIVE: resb 1

read_fail: db "Error reading stage 2", 0
invalid_mbr: db "Invalid partition table", 0
no_os: db "No OS found", 0
no_int13_ext: db "INT 13h extensions not found", 0

times 440-($-$$) db 0

part_table:
disk_id: dd 0
reserved: dw 0
part_1:
	.attrib db 0
	.chs_start times 3 db 0
	.part_type db 0
	.chs_end times 3 db 0
	.lba_start dd 0
	.sect_count dd 0
part_2:
	.attrib db 0
	.chs_start times 3 db 0
	.part_type db 0
	.chs_end times 3 db 0
	.lba_start dd 0
	.sect_count dd 0
part_3:
	.attrib db 0
	.chs_start times 3 db 0
	.part_type db 0
	.chs_end times 3 db 0
	.lba_start dd 0
	.sect_count dd 0
part_4:
	.attrib db 0
	.chs_start times 3 db 0
	.part_type db 0
	.chs_end times 3 db 0
	.lba_start dd 0
	.sect_count dd 0
signature: dw 0xaa55
