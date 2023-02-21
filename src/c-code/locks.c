#include "locks.h"
#include "heap.h"

semaphore_t* semaphore_create(int max_count) {
    semaphore_t * semaphore;

    semaphore = malloc(sizeof(semaphore_t));
    if(semaphore != 0) {
        semaphore->max_count = max_count;
        semaphore->current_count = 0;
        semaphore->first_waiting_task = 0;
        semaphore->last_waiting_task = 0;
    }

	return semaphore;
}

semaphore_t* mutex_create() {
	return semaphore_create(1);
}
