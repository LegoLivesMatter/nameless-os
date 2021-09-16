; Some routines for managing the A20 line

; Check if A20 is enabled
; Based on <https://wiki.osdev.org/A20_Line#Testing_the_A20_line>
check_a20:
	pushf
	push ds
	push es
	push di
	push si

	cli

	xor ax, ax
	mov es, ax

	not ax
	mov ds, ax

	mov di, 500h
	mov si, 510h

	mov al, byte [es:di]
	push ax

	mov al, byte [ds:si]
	push ax

	mov byte [es:di], 0
	mov byte [ds:si], 0FFh

	cmp byte [es:di], 0FFh

	pop ax
	mov byte [ds:si], al
	
	pop ax
	mov byte [es:di], al

	mov ax, 0
	je .exit
	mov ax, 1

.exit
	pop si
	pop di
	pop es
	pop ds
	popf
	ret

a20_i8042_wait
	in al, 64h
	test al, 2
	jnz a20_i8042_wait
	ret

a20_i8042_wait2
	in al, 64h
	test al, 1
	jnz a20_i8042_wait2
	ret

enable_a20
	mov ax, 2401h
	int 15h
	call check_a20
	cmp ax, 1
	jne .fail
.success
	ret
.fail
	cli ; disable interrupts

	call a20_i8042_wait
	mov al, 0ADh ; disable 1st PS/2 port
	out 64h, al

	call a20_i8042_wait
	mov al, 0D0h ; read controller output port
	out 64h, al

	call a20_i8042_wait2
	in al, 60h ; do the actual read
	push ax ; save the register

	call a20_i8042_wait
	mov al, 0D1h ; write byte to output port
	out 64h, al

	call a20_i8042_wait
	pop ax ; restore the register
	or al, 2 ; toggle A20 bit
	out 60h, al

	call a20_i8042_wait
	mov al, 0AEh ; reenable 1st PS/2 port
	out 64h, al

	call a20_i8042_wait
	sti ; enable interrupts
	ret

	call check_a20
	cmp ax, 1
	jne .fail2
	ret
.fail2
	mov si, A20_FAIL
	call print
	jmp $

A20_FAIL db "A20 enable fail, not booting!", 0
