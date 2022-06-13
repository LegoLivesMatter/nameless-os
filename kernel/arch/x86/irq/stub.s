; Stubs for interrupt handlers

bits 32
section .text

extern int_handler

int_common:
	cld
	call int_handler
	add esp, 4
	popad
	iretd

%macro INTERRUPT 1
global int%1
int_%1:
	pushad
	push dword %1
	jmp int_common
%endmacro

%assign i 0
%rep 48
INTERRUPT i
%assign i i+1
%endrep

; Define flat table containing addresses of handlers
section .rodata
global _int_handler_table
_int_handler_table:
	%macro INT 1
		dd int_%1
	%endmacro

	%assign i 0
	%rep 48
	INT i
	%assign i i+1
	%endrep
