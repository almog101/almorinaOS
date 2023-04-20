global	_syscall

[bits 64]
_syscall:
	mov 	r9, rdi		; syscall num
	mov 	rdi, rsi	; arg0
	mov 	rsi, rdx	; arg1
	mov 	rdx, rcx 	; arg2
	mov 	rcx, r8 	; arg3

	int 	0x80

	; mov 	rax, rdi
	ret
