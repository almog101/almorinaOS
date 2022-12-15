global start
extern long_mode_start

jmp start

section .bss
align 4096
page_table_l4:
	resb 4096
page_table_l3:
	resb 4096
page_table_l2:
	resb 4096

;define the stack
stack_bottom:
	resb 4096 * 4
stack_top:

section .rodata
gdt64:
	dq 0 ; zero entry
.code_segment: equ $ - gdt64
	dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; code segment
.pointer:
	dw $ - gdt64 - 1 ; length
	dq gdt64 ; address

section .text
[bits 32]
; a subroutine that checks if the kernel was loaded by a multiboot loader
detect_multiboot:
	cmp eax, 0x36d76289 ; a magic number used by the bootloader to mark multiboot
	jne .no_multiboot
	ret
.no_multiboot:
	mov al, "M"
	jmp error

; a subroutine that checks if the cpu as
; it does that by checking if it can flip the id bit in the flags register
detect_cpuid:
	pushfd
	pop eax

	mov ecx, eax
	
	xor eax, 1 << 21
	
	push eax
	popfd
	
	pushfd
	pop eax
	
	push ecx
	
	popfd
	cmp eax, ecx
	je .no_cpuid
	ret
.no_cpuid:
	mov al, "C"
	jmp error

detect_long_mode:
	mov eax, 0x80000000 ; a magic number that the cpuid uses mark the cpu if it as extended processor info
	cpuid
	cmp eax, 0x80000001 ; if true, cpuid will put in eax a number greater then the magic number
	jb .no_long_mode

	mov eax, 0x80000001 ; a magic number that the cpuid uses mark the cpu if it is supporting long mode
	cpuid
	test edx, 1 << 29 ; cpuid will return a number into dx, and if the lm bit is set, long mode is supported
	jz .no_long_mode
	
	ret
.no_long_mode:
	mov al, "L"
	jmp error

setup_page_tables:
	mov eax, page_table_l3
	or eax, 0b11 ; present, writable
	mov [page_table_l4], eax
	
	mov eax, page_table_l2
	or eax, 0b11 ; present, writable
	mov [page_table_l3], eax

	mov ecx, 0 ; counter
.loop:

	mov eax, 0x200000 ; 2MiB
	mul ecx
	or eax, 0b10000011 ; present, writable, huge page
	mov [page_table_l2 + ecx * 8], eax

	inc ecx ; increment counter
	cmp ecx, 512 ; checks if the whole table is mapped
	jne .loop ; if not, continue

	ret

enable_paging:
	; pass page table location to cpu
	mov eax, page_table_l4
	mov cr3, eax

	; enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; enable long mode
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret

error:
	; print "ERR: X" where X is the error code
	mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

start:
	mov esp, stack_top ; move the stack location to stack register

	call detect_multiboot
	call detect_cpuid
	call detect_long_mode

	call setup_page_tables
	call enable_paging

	lgdt [gdt64.pointer]
	jmp gdt64.code_segment:long_mode_start

	hlt

extern kernel_main
section .text
[bits 64]

long_mode_start:
    ; load null into all data segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

	call kernel_main
    jmp $
