%define CURRENT_ADDR cs:di

print_word:
	pusha ; push all regs to stack
	mov di, HEX_OUT+5 ; set destination index to last char in HEX_OUT
	mov ax, dx ; copy argument to accumulator
	and al, 00001111b ; extract the low nibble of the low half of the accumulator
	call .compare ; fill in the string with correct value
	dec di ; decrement destination index
	mov ax, dx ; copy argument to accumulator
	and al, 11110000b ; extract high nibble of low half of accumulator
	shr al, 4 ; shift high nibble to low nibble
	call .compare ; fill in string with correct value
	dec di
	mov al, ah ; copy high portion of accumulator to low
	and al, 00001111b ; 
	call .compare
	dec di
	mov al, ah
	and al, 11110000b
	shr al, 4
	call .compare
	jmp .write ; write string

.write:
	popa ; get back our arguments
	mov dl, cl ; column
	mov dh, ch ; row
	pusha ; save registers
	mov al, 1 ; update cursor
	mov bh, 0 ; page number
	mov bl, 00000111b ; colors
	mov cx, 6 ; length of string
	push cs ; push code segment address
	pop es ; pop to extra segment address
	mov bp, HEX_OUT ; offset to extra segment of string
	mov ah, 13h ; write string
	int 10h ; BIOS interrupt
	popa
	ret ; return

.compare:
	cmp al, 0 ; compare al with 0
	jne .one ; if not equal, compare with 1
	mov byte [CURRENT_ADDR], '0' ; set character to ASCII 0
	ret ; return
.one:
	cmp al, 1
	jne .two
	mov byte [CURRENT_ADDR], '1'
	ret
.two:
	cmp al, 2
	jne .three
	mov byte [CURRENT_ADDR], '2'
	ret
.three:
	cmp al, 3
	jne .four
	mov byte [CURRENT_ADDR], '3'
	ret
.four:
	cmp al, 4
	jne .five
	mov byte [CURRENT_ADDR], '4'
	ret
.five:
	cmp al, 5
	jne .six
	mov byte [CURRENT_ADDR], '5'
	ret
.six:
	cmp al, 6
	jne .seven
	mov byte [CURRENT_ADDR], '6'
	ret
.seven:
	cmp al, 7
	jne .eight
	mov byte [CURRENT_ADDR], '7'
	ret
.eight:
	cmp al, 8
	jne .nine
	mov byte [CURRENT_ADDR], '8'
	ret
.nine:
	cmp al, 9
	jne .ten
	mov byte [CURRENT_ADDR], '9'
	ret
.ten:
	cmp al, 0Ah
	jne .eleven
	mov byte [CURRENT_ADDR], 'A'
	ret
.eleven:
	cmp al, 0Bh
	jne .twelve
	mov byte [CURRENT_ADDR], 'B'
	ret
.twelve:
	cmp al, 0Ch
	jne .thirteen
	mov byte [CURRENT_ADDR], 'C'
	ret
.thirteen:
	cmp al, 0Dh
	jne .fourteen
	mov byte [CURRENT_ADDR], 'D'
	ret
.fourteen:
	cmp al, 0Eh
	jne .fifteen
	mov byte [CURRENT_ADDR], 'E'
	ret
.fifteen:
	mov byte [CURRENT_ADDR], 'F'
	ret

HEX_OUT: db "0x0000"
