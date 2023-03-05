global	_syscall

extern 	rsp_register

[bits 64]
_syscall:
	push 	rcx		;argument 3
	push 	rdx		;argument 2
	push 	rsi		;argument 1
	push 	rdi		;syscall number

	;mov 	[rsp_register], rsp
	int 	0x80

	pop 	rdi
	pop 	rsi
	pop 	rdx
	pop 	rcx

	ret
