; Some routines for managing the A20 line

; Check if A20 is enabled
; Based on <https://wiki.osdev.org/A20_Line#Testing_the_A20_line>, slightly modified
check_a20
	pushf ; save EFLAGS
	; save segment and index registers
	push ds 
	push es
	push di
	push si

	cli ; disable interrupts
	xor ax, ax ; ax = 0
	mov es, ax ; es = 0

	not ax ; ax = FFFFh
	mov ds, ax ; ds = FFFFh

	mov di, 500h
	mov si, 510h

	mov al, [es:di] ; preserve contents of 0:500h
	mov ah, [ds:si] ; preserve contents of FFFFh:510h
	push ax ; push all that to stack

	mov byte [es:di], 0 ; set 0:500h to 0
	mov byte [ds:si], 0FFh ; set FFFFh:510h to FFh

	cmp byte [es:di], 0FFh ; compare 0:500h with FFh
					  ; if A20 is enabled, this will not be equal, but if A20 is disabled it will be

	pop ax ; pop old contents of the 2 addresses
	mov [ds:si], ah ; restore contents of FFFFh:510h
	mov [es:di], al ; restore contents of 0:500h

	xor ax, ax ; clear ax
	je .exit ; if A20 is disabled, return 0
	mov ax, 1 ; otherwise return 1

.exit
	pop si
	pop di
	pop es
	pop ds
	popf
	sti
	ret

