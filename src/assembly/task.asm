global          switch_to_task          ;; void SwitchToTask(PCB_t *task) __attributes__((cdecl));
global          GetCR3                  ;; unsigned int GetCR3(void);

extern      currentPCB
extern 	    switch_to_task

TOS     equ     0
VAS     equ     8
STS     equ     24

RUNNING_STATE   equ     1
WAITING_STATE   equ     2

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

        push    rax                     ;; save rax
        mov     rax, [rdi + STS]
        mov     rbx, RUNNING_STATE
        cmp     rax, rbx                ;; check if current process's state is running state
        jne     .cont
        
        mov     rbx, WAITING_STATE
        mov     [rdi + STS], rbx        ;; save waiting state in current process's state

.cont:

        pop     rax                     ;; get back rax value
	mov     rbp, rax
        mov     [currentPCB], rbp       ;; save the adress of current PCB

        mov     rsp, [rbp + TOS]        ;; load the next process's stack
        mov     rax, [rbp + VAS]        ;; load the next process's virtual address space

        mov     rbx, RUNNING_STATE
        mov     [rbp + STS], rbx        ;; save running state in next process's state

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
