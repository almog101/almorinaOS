global  get_level
global  get_stack_pointer

[bits 64]
get_level:
        mov     rax, cs
        ret

get_stack_pointer:
        mov     eax, esp
        ret