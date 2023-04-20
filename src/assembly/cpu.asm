global cpu_load_gdt
cpu_load_gdt:
  lgdt [rdi]
  ret

global cpu_load_tr
cpu_load_tr:
  ltr di
  ret

global cpu_reload_segments
cpu_reload_segments:
  push 0x08
  lea rax, [rel .reload]
  push rax
  retfq
.reload:
  mov ax, 0x10
  mov ss, ax
  mov ax, 0x00
  mov ds, ax
  mov es, ax
  ret

  ; set up the stack frame so we can call
  ; iretq to set our new cs register value
  push qword 0x10 ; new ss
  push rbp        ; rsp
  pushfq          ; flags
  push qword 0x08 ; new cs
  push rax        ; rip
  iretq

; Control Registers

global cpu_start_usermode
extern shell_main
cpu_start_usermode:
;enable system call extensions that enables sysret and syscall
	mov rcx, 0xc0000082
	wrmsr
	mov rcx, 0xc0000080
	rdmsr
	or eax, 1
	wrmsr
	mov rcx, 0xc0000081
	rdmsr
	mov edx, 0x00180008
	wrmsr
 
	mov rcx, shell_main ; to be loaded into RIP
	mov r11, 0x202 ; to be loaded into EFLAGS
	o64 sysret
