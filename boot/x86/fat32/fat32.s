; FAT32 driver
; BOOT_DRIVE, si and bp must be set up by the code using this driver.
; (Specifically, si must be a pointer to the partition's partition
; table entry and bp must be a pointer to the BPB.)
; Before reading any cluster chains, callers must call get_1st_data_sec
; and must be careful not to trash ecx after calling it.

; eax - start cluster number
; es:di - where to load the cluster chain
; RETURN: CF set = error
; NOTE: Cluster chain might be the root directory or a file.
read_cluster_chain:
	push eax
	push es
	push di
	.loop:
		call read_cluster
		jc .done
		cmp eax, 0xffffff7
		jl .loop
.done:
	pop di
	pop es
	pop eax
	ret

; eax - cluster number
; es:di - where to load the cluster (incremented automatically)
; RETURN: eax - next cluster number
;	  CF set = error
read_cluster:
	push ebx
	push ecx
	push edx
.loop:
	; get the first sector of the cluster to read
	push eax
	sub eax, 2
	xor ebx, ebx
	mov bl, BPB_SecPerClus
	mul ebx
	add eax, ecx
	mov ebx, eax
	pop eax

	; read the cluster
	push cx
	movzx cx, BPB_SecPerClus
	call read_sectors
	pop cx
	cmp dl, 0
	je .error

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
	xor ebx, ebx
	mov bl, 4 ; put 4 in EBX, doing this instead of "mov ebx, 4" saves us an entire byte
	mul ebx
	movzx ebx, word BPB_BytsPerSec
	div ebx
	movzx ebx, word BPB_RsvdSecCnt
	add eax, ebx
	add eax, [si]
	mov ebx, edx
	; reminder for myself: EAX is FAT sector number, (E)BX is offset into FAT sector
	
	; load the FAT sector we're looking for
	push di
	push ebx ; offset
	push es ; we want to read at 0:800, not STAGE3_SEGMENT:800
	push eax ; desired LBA
	xor ax, ax
	mov es, ax
	pop ebx ; pop LBA into EBX
	mov di, 0x800
	mov cx, 1
	call read_sectors
	cmp dl, 0
	; find the cluster we're looking for
	pop es ; restore STAGE3_SEGMENT
	pop ebx ; pop FAT offset back into EBX for cmp
	mov eax, [di+bx]
	pop di
	jne .end
	; cleanup and return
.error:
	stc
.end:
	pop edx
	pop ecx
	pop ebx
	ret

; es:di - where to load the sector(s)
; ebx - start LBA address
; cx - how many sectors to read
; RETURN: DL=0 means error
read_sectors:
	clc
	push ax
	push si
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
	mov dl, 1
	jnc .done
	mov dl, 0
.done:
	add sp, 16
	pop si
	pop ax
	ret

; ds:si - pointer to partition table entry
; RETURN: ecx - first data sector
get_1st_data_sec:
	push eax
	push ebx
	movzx ax, BPB_NumFats 
	mul dword BPB_FatSz32 ; space occupied by all FATs
	movzx ebx, word BPB_RsvdSecCnt
	add eax, ebx ; space occupied by the reserved area
	add eax, [si] ; space before the partition
	mov ecx, eax
	pop ebx
	pop eax
	ret

read_error: db "Read error", 0

BOOT_DRIVE: db 0
