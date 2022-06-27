; x86-specific routine for halting

bits 32
global halt

halt:
	cli
	hlt
	jmp $-1
