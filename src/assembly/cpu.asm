global  cpu_load_tr
global  gdt_flush     ; Allows the C code to link to this

gdt_flush:
  lgdt  [rdi]         ; Load the GDT with our '_gp' which is a special pointer
  mov   ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
  mov   ds, ax
  mov   es, ax
  mov   fs, ax
  mov   gs, ax
  mov   ss, ax
  ret                 ; Returns back to the C code!

cpu_load_tr:
  mov   ax, 0x28
  ltr   dx
  ret