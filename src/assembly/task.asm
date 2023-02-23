global          switch_to_task          ;; void SwitchToTask(PCB_t *task) __attributes__((cdecl));
global          GetCR3                  ;; unsigned int GetCR3(void);

extern          currentPCB
extern 	        switch_to_task
extern          end_of_ready_list
extern          postponed_tasks_counter
extern          postponed_tasks_flag
extern 			PIT_get_counter

TOS			equ     0
VIRT_ADDR	equ     8
NEXT		equ     16
STATE		equ     24
TIME		equ		32

READY_STATE     equ     0
RUNNING_STATE   equ     1

[bits 64]
switch_to_task:

        push    rbx
        push    rsi
        push    rdi
        push    rbp

        mov     rdi, [currentPCB]       ;; 'rdi' = adress of previous tasks PCB
        mov     [rdi + TOS], rsp        ;; save the top of the stack

        push    rax                     ;; save rax

        cmp     dword [postponed_tasks_counter], 0
        je      .no_postponed_tasks
        mov     dword [postponed_tasks_flag], 1
        ret

.no_postponed_tasks:

        mov     al, [rdi + STATE]
        mov     bl, RUNNING_STATE
        cmp     al, bl                ;; check if current process's state is running state
        jne     .not_running_state
        
        mov     bl, READY_STATE
        mov     [rdi + STATE], bl        ;; save waiting state in current process's state

        mov     rax, [end_of_ready_list]
        mov     [rax + NEXT], rdi
        mov     [end_of_ready_list], rdi

.not_running_state:
        pop     rax                     ;; get back rax value
		mov     rbp, rax
        mov     [currentPCB], rbp       ;; save the adress of current PCB

		;sub  rsp, 8         ; reserve space on stack for return value
		call PIT_get_counter
		movsd xmm0, [rsp]   ; move return value from stack to XMM0
		;add  rsp, 8         ; clean up stack

		movsd	[rbp + TIME], xmm0
        mov     rsp, [rbp + TOS]        ;; load the next process's stack
        mov     rax, [rbp + VIRT_ADDR]        ;; load the next process's virtual address space

        mov     bl, RUNNING_STATE
        mov     [rbp + STATE], bl        ;; save running state in next process's state

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
