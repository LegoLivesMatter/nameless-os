; Code for enabling paging before calling the kernel
; Identity maps the VGA framebuffer memory and high-half maps the kernel memory
bits 32

section .text

; The problem with this code is that it assumes that the kernel's various
; sections occupy a certain number of pages. As of writing it is correct, but as
; the kernel grows this code may fail to map those pages, which is not good. The
; solution to this is to use ELF instead of a flat binary. This was not done
; before because it would require paging, but now that paging works using ELF is
; a possibility which must be exploited.

enable_paging:
	push eax
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	pop eax
	ret

load_paging_structs:
	push eax
	push ebx
	xor ebx, ebx
.pt_low_loop:
	mov eax, ebx
	shl eax, 12
	or eax, 1|2 ; P and R/W flags
	mov [page_table_low+ebx*4], eax
	inc ebx
	cmp ebx, 0x100
	jl .pt_low_loop

	xor ebx, ebx
.pt_high_ro_loop:
	mov eax, ebx
	add eax, 0x100
	shl eax, 12
	or eax, 1 ; P flag
	mov [page_table_high+ebx*4], eax
	inc ebx
	cmp ebx, 0x2
	jl .pt_high_ro_loop

	mov ebx, 0x2
.pt_high_rw_loop:
	mov eax, ebx
	add eax, 0x100
	shl eax, 12
	or eax, 1|2 ; P and R/W flags
	mov [page_table_high+ebx*4], eax
	inc ebx
	cmp ebx, 0x9
	jl .pt_high_rw_loop

	mov eax, page_table_low
	and eax, 0xfffff000
	or eax, 1|2
	mov [page_directory], eax

	mov eax, page_table_high
	and eax, 0xfffff000
	or eax, 1|2
	mov [page_directory+768*4], eax

	mov eax, page_directory
	mov cr3, eax
	pop ebx
	pop eax
	ret

section .data
align 4096
page_table_low:
	times 1024 dd 0
page_table_high:
	times 1024 dd 0
page_directory:
	times 1024 dd 0
