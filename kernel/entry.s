; when our kernel source has functions before _start and we blindly transfer control to the beginning
; of the kernel binary, it will wrongly execute the other functions
; the solution is to link a small assembly routine (this) which executes a known label within the kernel
; so that this routine comes before the kernel in the resulting binary
; we cannot link the boot sector code and the kernel because the former needs to be a raw binary, while the
; kernel is compiled into an ELF object file which contains some metadata on the kernel code

	bits 32 ; this is started in protected mode
	extern _start ; the known label in the kernel source we will execute is, well, not in this assembly routine

	call _start ; call the known label
	jmp $ ; it should never return, but hang forever if it does
