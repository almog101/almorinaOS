extern sse_enable

[bits 64]
sse_enable: ; int sse_enable(void)
	mov eax, 0x1
	cpuid
	test edx, 1<<25
	jz .noSSE

	;now enable SSE and the like
	mov rax, cr0
	and ax, 0xFFFB		;clear coprocessor emulation CR0.EM
	or ax, 0x2			;set coprocessor monitoring  CR0.MP
	mov cr0, rax
	mov rax, cr4
	or ax, 3 << 9		;set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
	mov cr4, rax
	mov rax, 0
	ret

.noSSE:
	mov rax, 1
	ret
