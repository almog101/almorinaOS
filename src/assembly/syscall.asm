global syscall
[bits 64]
syscall:
	push ax
	push bx

	mov bx, di
	mov ax, si
	mov bh, al

	int 0x80

	pop bx
	pop ax
	ret
