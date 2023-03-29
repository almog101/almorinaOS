global cpu_load_gdt
global cpu_load_tr
global cpu_reload_segments

cpu_load_gdt:
  lgdt [rdi]
  ret

cpu_load_tr:
  ltr di
  ret

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
