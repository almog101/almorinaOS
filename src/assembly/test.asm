section .text
global  get_level
global  _add

[bits 64]
get_level:
        mov     rax, cs
        ret
