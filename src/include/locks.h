#include "scheduler.h"
typedef struct {     
	int max_count;
	int current_count;
	PCB_t *first_waiting_task;
	PCB_t *last_waiting_task;
} semaphore_t;

semaphore_t* semaphore_create(int max_count);
semaphore_t* mutex_create();
