#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS       256

#define READY_STATE     0
#define RUNNING_STATE   1
#define PAUSED          2

// Process Control Block
typedef struct PCB_t 
{
    uint64_t tos;       // kernel stack top 
    uint64_t virtAddr;  // virtual address space
    struct PCB_t *next; // next task
    int state;          // ready to run / running / waiting for something
    int used;           // is available
	int switch_time;
} PCB_t;

extern PCB_t* currentPCB;
extern PCB_t  pcbArray[MAX_TASKS];
extern PCB_t* end_of_ready_list;
extern int postponed_tasks_counter;
extern int postponed_tasks_flag;

extern void switch_to_task(PCB_t *task) __attribute__((cdecl));
extern unsigned int GetCR3(void);
void process_update_time_used();

extern void scheduler_init(void);
extern PCB_t *process_create(void (*ent)());
void test_scheduler();

void schedule();

#endif
