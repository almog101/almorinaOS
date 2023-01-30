#ifndef HEAP_H
#define HEAP_H

#include "stdint.h"
#include "stdbool.h"

struct mem_segment {
	uint64_t 			len;
	bool 				free;
	struct mem_segment*	next; 
}typedef mem_segment_t;

extern void initialize_heap(uint64_t addr, uint64_t size);
extern void* malloc(uint64_t size);
void free(void* ptr);

int initialize_memory(unsigned long magic, unsigned long addr);
#endif

