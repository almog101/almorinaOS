// preemptive round robin scheduler

#include "scheduler.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "pit.h"
#include "locks.h"
#include "shell.h"

#define PUSH(tos,val) (*(-- tos) = val)
#define MAX_TASK_TIME 100

PCB_t* currentPCB = 0;
PCB_t* sceduale_task = 0;
PCB_t  pcbArray[MAX_TASKS];
PCB_t* start_of_ready_list = 0;
PCB_t* end_of_ready_list = 0;
PCB_t* terminated_task_list = 0;

bool is_first = true;
int IRQ_disable_counter = 0;
int postponed_tasks_counter = 0;
int postponed_tasks_flag = 0;

PCB_t* idle_task_p;

void idle_task(void)
{
    for(;;)
        __asm__("hlt");
}

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

    task->state = TASK_STATE_READY;

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
    if (start_of_ready_list == 0 || currentPCB == idle_task_p)
        switch_to_task(task);
    else
    {
		add_ready_process(task);
    }
    unlock_scheduler();
}

void pcb_free(PCB_t *pcb)
{
    if (pcb < &pcbArray[0] || pcb >= &pcbArray[MAX_TASKS]) 
        return;
    pcb->used = 0;
}

void free_task(PCB_t* task)
{
    // free the kernel stack top & free the task
    pcb_free(task);
}

void tasks_cleaner(void)
{
    PCB_t* task;

    lock_task();
    while (terminated_task_list != 0)
    {
        task = terminated_task_list;
        terminated_task_list = task->next;
        free_task(task);
    }
    unlock_task();
}

void terminate_task(void)
{
    // add clear memory in user space

    lock_task();
    lock_scheduler();

    currentPCB->next = terminated_task_list;
    terminated_task_list = currentPCB;

    unlock_scheduler();
    block_task(TASK_STATE_TERMINATED);
    // Make sure the cleaner task isn't paused
    tasks_cleaner();
	
    unlock_task();
}

/** TODO: add comments
initialize PCB array with 
*/
void scheduler_init(void) 
{
    for (int i = 1; i < MAX_TASKS; i ++)
        pcbArray[i].used = 0;

    pcbArray[0].used = 1;
    pcbArray[0].switch_time = 0;
    pcbArray[0].tos = 0;
    pcbArray[0].virtAddr = GetCR3();
    pcbArray[0].state = TASK_STATE_RUNNING;
    pcbArray[0].next = (PCB_t *)0;

    currentPCB = &pcbArray[0];
    start_of_ready_list = &pcbArray[0];
    end_of_ready_list = &pcbArray[0];
}

PCB_t *pcb_alloc(void)
{
    for (int i = 0; i < MAX_TASKS; i ++) {
        if (pcbArray[i].used == 0) {
            pcbArray[i].used = 1;
            pcbArray[i].switch_time = 0;
            pcbArray[i].tos = ((0x181 + i) << 12);  // also allocate a stack here
            pcbArray[i].next = &pcbArray[0];
            pcbArray[i].state = TASK_STATE_READY;

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

static void process_startup(void) 
{
	lock_scheduler();
	unlock_scheduler();
}

PCB_t *process_create(void (*ent)())
{
    PCB_t *rv = pcb_alloc();

    if (!rv) 
        return rv;

    uint64_t *tos = (uint64_t*)rv->tos;
    PUSH(tos, (uint64_t)process_startup);   // startup function
    PUSH(tos, (uint64_t)ent);               // entry point
	PUSH(tos, 0);   // RBP
    PUSH(tos, 0);   // RDI
    PUSH(tos, 0);   // RSI
    PUSH(tos, 0);   // RBX

    rv->tos = (uint64_t)tos;
    rv->virtAddr = GetCR3();

    return rv;
}

void irq_schedule_handler(void)
{

    lock_scheduler();

	double current_time = PIT_get_counter();
	PCB_t* task = 0;
    for (int i = 1; i < sizeof(pcbArray)/sizeof(PCB_t); i ++)
	{
		task = &pcbArray[i];
		if (task->used == 0)
			continue;

		if (task->state == TASK_STATE_TERMINATED)
			continue;

		if ( current_time - task->switch_time < MAX_TASK_TIME ){
			 if (task->state == TASK_STATE_PAUSED)
				add_ready_process(task);
		}
		else {
			task->state = TASK_STATE_PAUSED;
		}
	}

    unlock_scheduler();
	schedule();
}

/*       -----Testing-----           */

char pch = 'A';
int process_count = 0;

void Process(void)
{
    char ch = pch ++;
	process_count++;

	int count = 0;
    for (;;) {
        if (currentPCB->state == TASK_STATE_RUNNING) putc(ch);
        else putc(ch - 'A' + 'a');
		Sleep(2000);

		if (count++ >= 10) {
			process_count--;
			terminate_task();
			break;
		}
    }
}

void test_scheduler()
{
    scheduler_init();
	sceduale_task = currentPCB;

	PCB_t* p1 = process_create(Process);
	PCB_t* p2 = process_create(Process);
    PCB_t* p3 = process_create(Process);

	while (process_count == 0);

	while(process_count > 0) {
		Sleep(2000);
	}

	puts("Test ended\n");
}

void schedule()
{
    if (postponed_tasks_counter != 0)
    {
        postponed_tasks_flag = 1;
        return;
    }

    if (start_of_ready_list != 0)
    {
        PCB_t* task = start_of_ready_list;
        start_of_ready_list = start_of_ready_list->next;

        if (task == idle_task_p)
        {
            if (start_of_ready_list != 0)
            {   
                task = start_of_ready_list;
                idle_task_p->next = task->next;
                start_of_ready_list = idle_task;
            }
            else if (currentPCB->state == TASK_STATE_RUNNING)
                return;
        }

		if (task->state != TASK_STATE_TERMINATED)
			switch_to_task(task);
    }
}
