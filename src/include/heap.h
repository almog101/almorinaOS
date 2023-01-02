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
}typedef memory_segment_t;

// functions to allocate memory
extern void initialize_heap(uint64_t addr, uint64_t size);
extern void* malloc(uint64_t size);

// functions to free memory
void free(void* address);

#endif
