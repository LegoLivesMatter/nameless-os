; x86 bootloader for nameless-os, FAT32 VBR portion
; This is what's going to be on most USB sticks and HDDs, for now

bits 16
org 0x7c00
cpu 686

STAGE3_ADDRESS equ 0x8000
STAGE3_SEGMENT equ STAGE3_ADDRESS >> 4
STAGE3_OFFSET equ STAGE3_ADDRESS & 0xf

%include "fat32-structs.s"

_start:
	jmp short real_start
	nop

times 0x57 db 0 ; skip past BPB

real_start:
	sti
	; no need to set up segments and stack again, because MBR did it for us
	mov bp, 0x7c00

	; we expect the boot drive to be in DL and our partition table entry in DS:SI
	mov [BOOT_DRIVE], dl

	; calculate the 1st sector of the data area
	call get_1st_data_sec

.load_root:
	; load the root directory
	mov ax, 0x1000
	mov es, ax
	xor di, di
	mov eax, BPB_RootClus
	call read_cluster_chain

	push cx
	mov cx, 11
	push si
.find_stage_3:
	mov si, STAGE3_NAME
	cmp byte [es:di], 0 ; we have no more entries to look at
	je .stage3_missing
	cmp byte [es:di], 0xe5 ; the entry was only previously used and as such not worth looking at
	je .increment
	repe cmpsb
	je .stage3_found
.increment:
	add di, 32
	jno .find_stage_3
	mov bx, es
	add bx, 0x1000
	mov es, bx
.stage3_found:
	pop si
	pop cx
	add di, 9 ; knowing that cmpsb incremented this by 11, we can also increment it by 9
		       ; right here so we can use 1 less offset below
	; stage 3 has been found and ES:DI points to its directory entry
	mov ax, [es:di] ; load high half of the 1st cluster
	shl eax, 16
	mov ax, [es:di+(dir_entry.firstcluslo-dir_entry.firstclushi)] ; load low half of the 1st cluster
	mov bx, STAGE3_SEGMENT
	mov es, bx
	mov di, STAGE3_OFFSET
	call read_cluster_chain ; read stage 3
	mov ds, bx
	call STAGE3_SEGMENT:STAGE3_OFFSET ; call stage 3
	jmp .halt ; halt in case we return, which should never happen

.stage3_missing:
	mov si, stage3_missing
	call print
.halt:
	hlt
	jmp short $-1

; ds:si - string
print:
	pusha
	mov ah, 0xe
	xor bh, bh
.loop:
	lodsb
	cmp al, 0
	je .done
	int 0x10
	jmp .loop
.done:
	mov al, 0xd
	int 0x10
	mov al, 0xa
	int 0x10
	popa
	ret

%include "fat32.s"

stage3_missing: db "LOADER.BIN is missing", 0
STAGE3_NAME: db "LOADER  BIN"

times 510-($-$$) db 0
dw 0xaa55
