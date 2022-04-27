bits 16
cpu 686
org 0x0

%macro print 1

	mov si, %1
	call print_str
	
%endmacro

_start:
	print string
	hlt
	jmp $-1
	
print_str:
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
	popa
	ret

string: db "Hello from LOADER.BIN!", 0xd, 0xa, 0
