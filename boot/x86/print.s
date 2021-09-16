; cs:si - pointer to ASCIIZ string
print
	pushf
	push ax
	push bx
	push si
	xor bh, bh
	mov ah, 0Eh

.loop
	mov al, [cs:si]
	cmp al, 0
	je .done
	int 10h
	inc si
	jmp .loop

.done
	mov al, 0Dh
	int 10h
	mov al, 0Ah
	int 10h
	pop si
	pop bx
	pop ax
	popf
	ret
