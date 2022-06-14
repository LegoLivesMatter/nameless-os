; Stubs for interrupt handlers

bits 32
section .text

extern int_handler

int_common:
	cld
	push esp
	call int_handler
	add esp, 12
	popad
	iretd

%macro INTERRUPT 1
global int_%1
int_%1:
	push dword 0
	pushad
	push dword %1
	jmp int_common
%endmacro

%macro INTERRUPT_ERR 1
global int_%1
int_%1:
	pushad
	push dword %1
	jmp int_common
%endmacro

INTERRUPT 0
INTERRUPT 1
INTERRUPT 2
INTERRUPT 3
INTERRUPT 4
INTERRUPT 5
INTERRUPT 6
INTERRUPT 7
INTERRUPT_ERR 8
INTERRUPT 9
INTERRUPT_ERR 10
INTERRUPT_ERR 11
INTERRUPT_ERR 12
INTERRUPT_ERR 13
INTERRUPT_ERR 14
INTERRUPT 15
INTERRUPT 16
INTERRUPT_ERR 17
INTERRUPT 18
INTERRUPT 19
INTERRUPT 20
INTERRUPT_ERR 21
INTERRUPT 22
INTERRUPT 23
INTERRUPT 24
INTERRUPT 25
INTERRUPT 26
INTERRUPT 27
INTERRUPT 28
INTERRUPT_ERR 29
INTERRUPT_ERR 30
INTERRUPT 31
INTERRUPT 32
INTERRUPT 33
INTERRUPT 34
INTERRUPT 35
INTERRUPT 36
INTERRUPT 37
INTERRUPT 38
INTERRUPT 39
INTERRUPT 40
INTERRUPT 41
INTERRUPT 42
INTERRUPT 43
INTERRUPT 44
INTERRUPT 45
INTERRUPT 46
INTERRUPT 47

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
