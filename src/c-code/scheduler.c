#include "scheduler.h"
#include <stdint.h>
#include <stdio.h>
#include "pit.h"

#define PUSH(tos,val) (*(-- tos) = val)

PCB_t *currentPCB;
PCB_t pcbArray[MAX_TASKS];
static double lastCounter;

void scheduler_init(void) 
{
    for (int i = 0; i < MAX_TASKS; i ++) pcbArray[i].time_used = 0;

    pcbArray[0].time_used = 1;
    pcbArray[0].tos = 0;
    pcbArray[0].virtAddr = GetCR3();
    pcbArray[0].state = 0;
    pcbArray[0].next = (PCB_t *)0;

    currentPCB = &pcbArray[0];
    lastCounter = PIT_get_counter();
}

PCB_t *pcb_alloc(void)
{
    for (int i = 0; i < MAX_TASKS; i ++) {
        if (pcbArray[i].time_used == 0) {
            pcbArray[i].time_used = 1;
            pcbArray[i].tos = ((0x181 + i) << 12);  // also allocate a stack here
            return &pcbArray[i];
        }
    }

    return (PCB_t *)0;
}


void pcb_free(PCB_t *pcb)
{
    if (pcb < &pcbArray[0] || pcb >= &pcbArray[MAX_TASKS]) return;
    pcb->time_used = 0;
}


static void process_startup(void) 
{
}

PCB_t *process_create(void (*ent)())
{
    PCB_t *rv = pcb_alloc();

    if (!rv) return rv;


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

void process_update_time_used() {
    double counter = lastCounter;
    lastCounter = PIT_get_counter();
    currentPCB->time_used += (lastCounter - counter);
}

/*       -----Testing-----           */

PCB_t *A;
PCB_t *B;
PCB_t *C;

void ProcessB(void)
{
    while (1) {
        putc('B');
        switch_to_task(C);
    }
}

void ProcessC(void)
{
    while (1) {
        putc('C');
        switch_to_task(A);
    }
}

void test_scheduler()
{
    A = currentPCB;
    B = process_create(ProcessB);
    C = process_create(ProcessC);

	for (int i = 0; i<5; i++)
	{
        putc('A');
        switch_to_task(B);
    }
	putc('\n');
}
