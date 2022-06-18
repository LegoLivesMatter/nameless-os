; Initial GDT for kernel, which wants a flat code and data descriptor

section .rodata

gdt:
	dw .end-.start-1
	dd .start
.start: dq 0 ; null descriptor
.code:
	dw 0xffff
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0
.data:
	dw 0xffff
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0
.end:

