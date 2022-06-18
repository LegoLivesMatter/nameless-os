bits 16
cpu 686
org 0x1800

%include "../fat32/fat32-structs.s"

%macro print 1
	push si
	mov si, %1
	call print_str
	pop si
%endmacro

_start:
	mov [BOOT_DRIVE], dl
	call enable_unreal
	print begin
	call check_a20
	jc .a20_enabled
	call enable_a20
	jnc .a20_enable_fail
.a20_enabled:
	print a20_enabled
	call get_1st_data_sec
	mov ax, 0x1000
	mov es, ax
	mov eax, BPB_RootClus
	xor di, di
	call read_cluster_chain
	jc critical_error

	push cx
	push si
.find_kernel:
	mov si, kernel_name
	cmp byte [es:di], 0 ; end of root directory
	je .kernel_missing
	cmp byte [es:di], 0xe5 ; unused entry
	je .increment
	mov cx, 11
	push si
	push di
	repe cmpsb
	pop di
	pop si
	je .kernel_found
.increment:
	add di, 32
	jno .find_kernel
	mov bx, es
	add bx, 0x1000
	mov es, bx
	jmp .find_kernel
.kernel_found:
	pop si
	pop cx
	print kernel_found
	mov ax, [es:di+dir_entry.firstclushi]
	shl eax, 16
	mov ax, [es:di+(dir_entry.firstcluslo)]
	call print_dword
	mov edi, 0x100000
	print kernel_loading
	call read_clus_chain_unreal ; load kernel
	print kernel_loaded
	
	call get_e820_map

	cli
	lgdt [gdt]
	mov eax, cr0
	or al, 1
	mov cr0, eax
	jmp 0x8:in_protected
.kernel_missing:
	print missing_kernel
	jmp .halt

.a20_enable_fail:
	print a20_fail
.halt:
	hlt
	jmp $-1

; eax - start cluster
; edi - address to load to
read_clus_chain_unreal:
	push eax
	push edi
.loop:
	push di
	push es
	push ax
	xor di, di
	mov ax, 0x1000
	mov es, ax
	pop ax
	call read_cluster
	pop es
	pop di
	jc .done
	push esi
	push eax
	push ebx
	push ecx
	mov esi, 0x10000
	xor ebx, ebx
	movzx eax, word BPB_BytsPerSec
	movzx bx, byte BPB_SecPerClus
	mul ebx
	mov ecx, eax
	call memcpy
	add edi, ecx
	pop ecx
	pop ebx
	pop eax
	pop esi
	cmp eax, 0xffffff7
	jl .loop
.done:
	pop edi
	pop eax
	ret

; esi - copy from
; edi - copy to
; ecx - bytes to copy
memcpy:
	push esi
	push edi
	push ecx
	push dx

.loop:
	mov dl, [esi]
	mov [edi], dl
	dec ecx
	cmp ecx, 0
	je .done
	inc esi
	inc edi
	jmp .loop
.done:
	pop dx
	pop ecx
	pop edi
	pop esi
	ret


%include "unreal.s"
%include "a20.s"
%include "../fat32/fat32.s"
%include "gdt.s"
%include "print.s"
%include "e820.s"

in_protected:
bits 32
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax

	call load_paging_structs
	call enable_paging

	jmp 0x8:0xc0000000
	nop

kernel_name: db "KERNEL  BIN"
begin: db "Nameless Bootloader revision ", GIT_REVISION, 0xd, 0xa, 0
a20_enabled: db "A20 has been enabled", 0xd, 0xa, "Searching for kernel...", 0xd, 0xa, 0
a20_fail: db "Failed to enable A20, giving up!", 0xd, 0xa, 0
crit_err: db "A critical error occurred, dumping registers now: ", 0xd, 0xa, 0
kernel_found: db "Found kernel at cluster ", 0
kernel_loading: db 0xd, 0xa, "Loading kernel...", 0xd, 0xa, 0
kernel_loaded: db "Kernel successfully loaded.", 0xd, 0xa, "Setting up kernel environment and running kernel...", 0xd, 0xa, 0
missing_kernel: db "Could not find KERNEL.BIN", 0xd, 0xa, 0
eax_s: db "EAX: ", 0
ebx_s: db "EBX: ", 0
ecx_s: db "ECX: ", 0
edx_s: db "EDX: ", 0
esi_s: db "ESI: ", 0
edi_s: db "EDI: ", 0
cs_s: db "CS: ", 0
ds_s: db "DS: ", 0
es_s: db "ES: ", 0
ss_s: db "SS: ", 0
space: db " ", 0
hex_delm: db "0x", 0
newline: db 0xd, 0xa, 0

%include "paging.s"
