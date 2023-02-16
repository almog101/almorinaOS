global      switch_to_task      ;; void SwitchToTask(PCB_t *task) __attributes__((cdecl));
global      GetCR3              ;; unsigned int GetCR3(void);

extern      currentPCB
extern 		switch_to_task
extern      process_update_time_used ;; void process_update_time_used()

TOS     equ         0
VAS     equ         8


[bits 64]
switch_to_task:
        push        rbx
        push        rsi
        push        rdi
        push        rbp

		push		rax
        call        process_update_time_used ;; update the time used field before swapping
		pop			rax

        mov         rdi,[currentPCB]        ;; `edi` = previous tasks PCB
        mov         [rdi+TOS],rsp           ;; save the top of the stack


        ;; -- load the next task's state


		mov			rbp, rax
        mov         [currentPCB],rbp        ;; save the current process PCB

        mov         rsp,[rbp+TOS]           ;; load the next process's stack
        mov         rax,[rbp+VAS]           ;; load the next process's virtual address space

        ;; -- fix tss.esp0 here

        mov         rcx,cr3                 ;; get the current task cr3

        cmp         rcx,rax                 ;; are the virtual address spaces the same
        je         .sameVAS 
        mov         cr3,rax                 ;; update to the new virtual address space

.sameVAS:
        pop         rbp
        pop         rdi
        pop         rsi
        pop         rbx

        ret                                 ;; this is the next task's `eip`

[bits 64]
GetCR3:
        mov         rax,cr3
        ret
