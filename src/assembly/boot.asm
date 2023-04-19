global start
extern long_mode_start

jmp start

; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

section .bss

;-----------------------------------

;ensure that page tables are page aligned
align 4096

;define 3 levels of page table
;reserve 0x1000 bits of uninitialized data for each
page_table_l4:
	resb 4096
page_table_l3:
	resb 4096
page_table_l2:
	resb 4096

;-----------------------------------

;define the stack
;reserve 0x1000 * 4 bits of uninitialized data for it
stack_bottom:
	resb 4096 * 4
stack_top:

; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

section .rodata

;-----------------------------------

gdt64:
	dq 0 ;zero entry

;set code segment
.code_segment: equ $ - gdt64
	dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)

;set length & address
.pointer:
	dw $ - gdt64 - 1
	dq gdt64

; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

section .text
[bits 32]

;-----------------------------------

;a subroutine that checks if the kernel was loaded by a multiboot loader
;if the bootloader wrote the magic number into eax register before laoding kernel
;then it is fine
detect_multiboot:
	;the magic number used by the bootloader to mark multiboot
	cmp eax, 0x36d76289
	jne .no_multiboot
	ret

;multiboot is not found - print error message "M"
.no_multiboot:
	mov al, "M"
	jmp error

;-----------------------------------

;a subroutine that checks if CPUID is supported by computer's processor
;we check if it can flip the id bit in the flags register
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

;CPUID is not found - print error message "C"
.no_cpuid:
	mov al, "C"
	jmp error

;-----------------------------------

;a subroutine that checks if long mode is availale to be used by computer's CPU
detect_long_mode:
	mov eax, 0x80000000	;a magic number that the cpuid uses mark the cpu if it as extended processor info
	cpuid
	cmp eax, 0x80000001	;if true, cpuid will put in eax a number greater then the magic number
	jb .no_long_mode

	mov eax, 0x80000001	;a magic number that the cpuid uses mark the cpu if it is supporting long mode
	cpuid
	test edx, 1 << 29	;cpuid will return a number into dx, and if the lm bit is set, long mode is supported
	jz .no_long_mode
	
	ret

;long mode is not supported - print error message "L"
.no_long_mode:
	mov al, "L"
	jmp error

;-----------------------------------

setup_page_tables:
	;map first PML4 entry to PDP table
	mov eax, page_table_l3
	or eax, 0b11 ; present, writable
	mov [page_table_l4], eax
	
	;map first PDP entry to PD table
	mov eax, page_table_l2
	or eax, 0b11 ; present, writable
	mov [page_table_l3], eax

	;counter
	mov ecx, 0

;loop that maps each PD entry to 2mIb page that starts at address 2MiB * ecx
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

;-----------------------------------

;print "ERR: X" where X is the error code
error:
	mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

;-----------------------------------

start:
	;move the stack location to stack register
	mov esp, stack_top

	push eax
	push ebx

	call detect_multiboot
	call detect_cpuid
	call detect_long_mode

	;set up basic paging up to 2MiB
	call setup_page_tables
	call enable_paging

	pop ebx
	pop eax

	;load the gdt to its register
	lgdt [gdt64.pointer]
	jmp gdt64.code_segment:long_mode_start

	hlt

; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

extern kernel_main
section .text
[bits 64]

;-----------------------------------

long_mode_start:
	
	mov rdi, rax
	mov rsi, rbx

    ;load null into all data segment registers
	mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

	;load our kernel
	call kernel_main
    jmp $
