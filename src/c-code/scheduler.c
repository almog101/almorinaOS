// cooperative round robin scheduler 

#include "scheduler.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define PUSH(tos,val) (*(-- tos) = val)

PCB_t* currentPCB;
PCB_t* sceduale_task;
PCB_t  pcbArray[MAX_TASKS];
PCB_t* start_of_ready_list = 0;
PCB_t* end_of_ready_list;

bool is_first = true;
int IRQ_disable_counter = 0;
int postponed_tasks_counter = 0;
int postponed_tasks_flag = 0;

void lock_task(void)
{
#ifndef SMP
    __asm__("cli");
    IRQ_disable_counter++;
    postponed_tasks_counter++;
#endif
}

void unlock_task(void)
{
#ifndef SMP
    postponed_tasks_counter--;

    if(postponed_tasks_counter == 0)
    {
        if(postponed_tasks_flag != 0) 
        {
            postponed_tasks_flag = 0;
            schedule();
        }
    }
    IRQ_disable_counter--;
    if(IRQ_disable_counter == 0)
        __asm__("sti");
#endif
}
 
void lock_scheduler(void) 
{
#ifndef SMP
    __asm__("cli");
    IRQ_disable_counter++;
#endif
}
 
void unlock_scheduler(void) 
{
#ifndef SMP
    IRQ_disable_counter--;
    if (IRQ_disable_counter == 0)
        __asm__("sti");
#endif
}

void block_task(int reason_for_waiting)
{
    lock_scheduler();
    currentPCB->state = reason_for_waiting;
    schedule();
    unlock_scheduler();
}

//
// -- Add a process to the list of ready tasks
//    ----------------------------------------
void add_ready_process(PCB_t *task)
{
    if (!task) return;

    task->state = READY_STATE;

    if (start_of_ready_list == 0) {
        start_of_ready_list = end_of_ready_list = task;
    } else {
        start_of_ready_list->next = task;
        end_of_ready_list = task;
    }
}

void unblock_task(PCB_t* task)
{
    lock_scheduler();
    if (start_of_ready_list == 0)
        switch_to_task(task);
    else
    {
		add_ready_process(task);
        //end_of_ready_list->next = task;
        //end_of_ready_list = task;
    }
    unlock_scheduler();
}

/** TODO: add comments
initialize PCB array with 
*/
void scheduler_init(void) 
{
    for (int i = 1; i < MAX_TASKS; i ++)
        pcbArray[i].used = 0;

    pcbArray[0].time_used = 1;
    pcbArray[0].tos = 0;
    pcbArray[0].virtAddr = GetCR3();
    pcbArray[0].state = RUNNING_STATE;
    pcbArray[0].next = (PCB_t *)0;

    currentPCB = &pcbArray[0];
    start_of_ready_list = &pcbArray[0];
    end_of_ready_list = &pcbArray[0];
}

PCB_t *pcb_alloc(void)
{
    for (int i = 0; i < MAX_TASKS; i ++) {
        if (pcbArray[i].time_used == 0) {
            pcbArray[i].time_used = 1;
            pcbArray[i].tos = ((0x181 + i) << 12);  // also allocate a stack here
            pcbArray[i].next = &pcbArray[0];
            pcbArray[i].state = READY_STATE;

            if(is_first)
            {
                start_of_ready_list = &pcbArray[i];
                end_of_ready_list = &pcbArray[i];
                is_first = false;
            }
            end_of_ready_list->next = &pcbArray[i];
            end_of_ready_list = &pcbArray[i];
            
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
	unlock_scheduler();
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
    rv->sleep_time = (unsigned long)-1;

    return rv;
}

void add_sleeping_process(PCB_t *task)
{
    if (!task) return;

    task->state = SLEEPING;

    if (sleepingListHead == (PCB_t *)0) {
        sleepingListHead = sleepingListTail = task;
    } else {
        sleepingListTail->next = task;
        sleepingListTail = task;
    }
}

void SleepUntil(unsigned long when)
{
    //LockAndPostpone();

    if (when < PIT_get_counter()) {
        //UnlockAndSchedule();
        return;
    }

    currentPCB->sleep_time = when;
    add_sleeping_process(currentPCB);

    //UnlockAndSchedule();    // -- this is OK because the scheduler structures are in order; worst case 
                            //    is a task change to itself.
    //BlockProcess(SLEEPING);
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

char pch = 'A';
void Process(void)
{
    char ch = pch ++;
    while (true) {
        if (currentPCB->state == RUNNING_STATE) putc(ch);
        else putc(ch - 'A' + 'a');
		putc('\n');

        block_task(PAUSED);
    }
}

void test_scheduler()
{
    scheduler_init();
	sceduale_task = currentPCB;

	PCB_t* p1 = process_create(Process);
	PCB_t* p2 = process_create(Process);
	process_create(Process);
	process_create(Process);
	process_create(Process);

    for (int j = 0; j<3; j++) {
        for (int i = 0; i < 4; i ++) {
            lock_scheduler();
            schedule();
            unlock_scheduler();
        }
		unblock_task(p1);
		unblock_task(p2);
    }
	puts("Test ended\n");
}

void schedule()
{
    if(postponed_tasks_counter != 0)
    {
        postponed_tasks_flag = 1;
        return;
    }

    if (start_of_ready_list != 0)
    {
        PCB_t* task = start_of_ready_list;
        start_of_ready_list = start_of_ready_list->next;
        switch_to_task(task);
    }
	else {
		switch_to_task(sceduale_task);
	}
}
