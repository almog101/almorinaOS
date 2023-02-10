#include "scheduler.h"
#include <stdint.h>
#include <stdio.h>

#define PUSH(tos,val) (*(-- tos) = val)

PCB_t* currentPCB;
PCB_t  pcbArray[MAX_TASKS];
PCB_t* startOfPcbArr;
PCB_t* endOfPcbArr = 0;

/** TODO: add comments
initialize PCB array with 
*/
void scheduler_init(void) 
{
    for (int i = 1; i < MAX_TASKS; i ++)
        pcbArray[i].used = 0;

    pcbArray[0].used = 1;
    pcbArray[0].tos = 0;
    pcbArray[0].virtAddr = GetCR3();
    pcbArray[0].state = RUNNING_STATE;
    pcbArray[0].next = (PCB_t *)0;

    currentPCB = &pcbArray[0];
    startOfPcbArr = &pcbArray[0];
}

PCB_t *pcb_alloc(void)
{
    for (int i = 0; i < MAX_TASKS; i++) 
    {
        if (pcbArray[i].used == 0) 
        {
            pcbArray[i].used = 1;
            pcbArray[i].tos = ((0x181 + i) << 12);  // also allocate a stack here
            pcbArray[i].next = &pcbArray[0];
            pcbArray[i].state = READY_STATE;

            if (i != 0)
                pcbArray[i-1].next = &pcbArray[i];

            endOfPcbArr = &pcbArray[i];
            return &pcbArray[i];
        }
    }

    return (PCB_t *)0;
}

void pcb_free(PCB_t *pcb)
{
    if (pcb < &pcbArray[0] || pcb >= &pcbArray[MAX_TASKS]) 
        return;
    pcb->used = 0;
}

static void process_startup(void) 
{
    /// TODO: ??
}

PCB_t *process_create(void (*ent)())
{
    PCB_t *rv = pcb_alloc();

    if (!rv) 
        return rv;

    uint64_t *tos = (uint64_t*)rv->tos;
    PUSH(tos, (uint64_t)process_startup);  // startup function
    PUSH(tos, (uint64_t)ent);        // entry point
	PUSH(tos, 0);           // RBP
    PUSH(tos, 0);           // RDI
    PUSH(tos, 0);           // RSI
    PUSH(tos, 0);           // RBX

    rv->tos = (uint64_t)tos;
    rv->virtAddr = GetCR3();

    return rv;
}

PCB_t *A;
PCB_t *B;
PCB_t *C;

void ProcessB(void)
{
    while (1)
	{
        putc('B');
        printf("\n[b: %d, ", currentPCB->state);
        printf("c: %d] ", currentPCB->next->state);
        schedule();
    }
}

void ProcessC(void)
{
    while (1)
	{
        putc('C');
        printf("\n[c: %d, ", currentPCB->state);
        printf("a: %d] ", currentPCB->next->state);
        schedule();
    }
}

void test_scheduler()
{
    scheduler_init();

    A = currentPCB;
    B = process_create(ProcessB);
    C = process_create(ProcessC);

	for (int i = 0; i < 2; i++)
	{
        putc('A');
        printf("\n[a: %d, ", currentPCB->state);
        printf("b: %d] ", currentPCB->next->state);
        schedule();
    }
}

void schedule()
{
    switch_to_task(currentPCB->next);
}
