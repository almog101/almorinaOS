#ifndef HEAP_H
#define HEAP_H

#include "stdint.h"
#include "stdbool.h"

struct memory_segment {
	uint64_t 				len;
	struct memory_segment* 	next_seg;
	struct memory_segment* 	prev_seg;
	struct memory_segment* 	next_free_seg;
	struct memory_segment* 	prev_free_seg;
	bool 					free;
}; 
typedef struct memory_segment memory_segment_t;

void initialize_heap(uint64_t addr, uint64_t size);
void* malloc(uint64_t size);


#endif
