global      switch_to_task              ;; void SwitchToTask(PCB_t *task) __attributes__((cdecl));
global      GetCR3                      ;; unsigned int GetCR3(void);

extern      currentPCB
extern 	    switch_to_task
TOS     equ         0
VAS     equ         8

[bits 64]
switch_to_task:

        ;; [save previous task's state]

        push    rbx
        push    rsi
        push    rdi
        push    rbp

        mov     rdi, [currentPCB]       ;; 'rdi' = adress of previous tasks PCB
        mov     [rdi + TOS], rsp        ;; save the top of the stack

        ;; -- load the next task's state

	mov     rbp, rax
        mov     [currentPCB], rbp       ;; save the adress of current PCB

        mov     rsp, [rbp + TOS]        ;; load the next process's stack
        mov     rax, [rbp + VAS]        ;; load the next process's virtual address space

        ;; -- fix tss.esp0 here

        mov     rcx, cr3                ;; get the current task cr3

        cmp     rcx, rax                ;; are the virtual address spaces the same
        je      .sameVAS 
        mov     cr3, rax                ;; update to the new virtual address space

.sameVAS:
        pop     rbp
        pop     rdi
        pop     rsi
        pop     rbx

        ret                             ;; this is the next task's `rip`

[bits 64]
GetCR3:
        mov         rax, cr3
        ret
