; FAT32 driver
; BOOT_DRIVE, part_table_entry and bp must be set up by the code using this driver.
; first_data_sec is to be set up by calling get_1st_data_sec.

; eax - start cluster number
; es:di - where to load the cluster chain
; NOTE: Cluster chain might be the root directory or a file.
read_cluster_chain:
	pusha
.loop:
	; get the first sector of the cluster to read
	push eax
	sub eax, 2
	xor ebx, ebx
	mov bl, BPB_SecPerClus
	mul ebx
	add eax, [first_data_sec]
	mov ebx, eax
	pop eax

	; read the cluster
	movzx cx, BPB_SecPerClus
	call read_sectors

	; increment the load offset (and segment if needed)
	push eax
	xor eax, eax
	movzx ax, BPB_SecPerClus
	mul word BPB_BytsPerSec
	add di, ax
	jno .get_next_cluster
	mov ax, es
	add ax, 0x1000
	mov es, ax

.get_next_cluster:
	pop eax
	; get FAT sector number and offset containing the next cluster number
	xor edx, edx
	mov ebx, 4
	mul dword ebx
	movzx ebx, word BPB_BytsPerSec
	div dword ebx
	movzx ebx, word BPB_RsvdSecCnt
	add eax, ebx
	push bp
	mov bp, [part_table_entry]
	add eax, [bp+part_entry.lba_start]
	pop bp
	mov ebx, edx
	; reminder for myself: EAX is FAT sector number, (E)BX is offset into FAT sector
	
	; load the FAT sector we're looking for

	push ebx ; offset
	push es ; we want to read at 0:1000, not STAGE3_SEGMENT:1000
	push eax ; desired LBA
	xor ax, ax
	mov es, ax
	pop ebx ; pop LBA into EBX
	mov di, 0x1000
	mov cx, 1
	call read_sectors
	
	; find the cluster we're looking for
	pop es ; restore STAGE3_SEGMENT
	pop ebx ; pop FAT offset back into EBX for cmp
	cmp dword [di+bx], 0xffffff7
	jge .done ; if cluster number is greater than or equal to the defective cluster value, we're done
		  ; TODO: should this perhaps be changed so that equal makes it error out?
	; otherwise, load the next sector number in eax and read again
	mov eax, [di+bx]
	jmp .loop
.done:
	; cleanup and return
	popa
	ret

; es:di - where to load the sector(s)
; ebx - start LBA address
; cx - how many sectors to read
read_sectors:
	pusha
	mov ah, 0x42
	push dword 0
	push dword ebx
	push es
	push di
	push cx
	push word 0x10
	mov si, sp
	mov dl, [BOOT_DRIVE]
	int 0x13
	jc .error
	add sp, 16
	popa
	ret
	.error:
		mov si, read_error
		call print
		hlt
		jmp $-1

; no arguments
get_1st_data_sec:
	push eax
	push ebx
	xor eax, eax
	movzx ax, BPB_NumFats 
	mul dword BPB_FatSz32 ; space occupied by all FATs
	movzx ebx, word BPB_RsvdSecCnt
	add eax, ebx ; space occupied by the reserved area
	mov bx, [part_table_entry]
	add eax, [bx+part_entry.lba_start] ; space before the partition
	mov [first_data_sec], eax
	pop ebx
	pop eax
	ret

read_error: db "Read error", 0

BOOT_DRIVE: db 0
part_table_entry: dw 0
first_data_sec: dd 0
