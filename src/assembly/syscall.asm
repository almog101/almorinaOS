global	_syscall

extern 	rsp_register

[bits 64]
_syscall:
	push 	rbx

	push 	rcx
	push 	rdx

	xor 	rbx, rbx
	mov 	rbx, rsi
	push 	rbx

	mov 	[rsp_register], rsp
	mov 	rbx, rdi

	int 	0x80

	pop 	rbx
	pop 	rdx
	pop 	rcx

	pop 	rbx
	ret

;rdi
;rsi
;rdx
;rcx