global	syscall

[bits 64]
syscall:
	push 	bx

	mov 	bx, di	;syscall number
	int 	0x80

	pop 	bx
	ret
