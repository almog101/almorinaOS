#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_TASKS       256

enum TaskState 
{
	TASK_STATE_READY,
	TASK_STATE_RUNNING,
	TASK_STATE_PAUSED,
	TASK_STATE_TERMINATED,
	TASK_STATE_WAITING_FOR_LOCK,
};

// Process Control Block
typedef struct PCB_t 
{
    uint64_t tos;       // kernel stack top 
    uint64_t virtAddr;  // virtual address space
    struct PCB_t *next; // next task
    uint8_t state;          // ready to run / running / waiting for something
    bool used;           // is available
	double switch_time;
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

void lock_scheduler(void);
void unlock_scheduler(void);

void schedule();

#endif
