#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#define MAX_TASKS       256

typedef struct PCB_t {
    uint64_t tos;
    uint64_t virtAddr;
    struct PCB_t *next;
    int state;
    int time_used;
} PCB_t;


extern PCB_t *currentPCB;
extern PCB_t pcbArray[MAX_TASKS];

extern void switch_to_task(PCB_t *task) __attribute__((cdecl));
extern unsigned int GetCR3(void);
void process_update_time_used();

extern void scheduler_init(void);
extern PCB_t *process_create(void (*ent)());
void test_scheduler();
#endif
