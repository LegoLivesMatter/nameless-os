; ES:BX - where to load
; DH - sector count
; DL - drive number

read_sectors:
	xor bl, bl
	mov di, disk_reading
	call print
	push dx
	mov ah, 02h
	mov al, dh
	mov ch, 0
	mov dh, 0
	mov cl, 2
	
	int 13h

	jc read_error

	pop dx
	cmp dh, al
	jne read_error
	ret
	
read_error:
	xor bl, bl
	mov di, disk_error
	call print
	mov dl, ah
	xor dh, dh
	call print_word
	mov dl, al
	call print_word
	jmp $

disk_reading db "Reading from disk...", 0
disk_error db "Failed to read from disk! Take a look at this error code:", 0

	ret
