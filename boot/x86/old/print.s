; ds:si - pointer to ASCIIZ string
print:
	pusha
	pushf
	cld
	xor bh, bh
	mov ah, 0Eh
.loop:
	lodsb
	cmp al, 0
	je .done
	int 10h
	jmp .loop
.done:
	mov al, 0Dh
	int 10h
	mov al, 0Ah
	int 10h
	popf
	popa
	ret
