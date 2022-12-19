#ifndef HEAP_H
#define HEAP_H

#include "stdint.h"
#include "stdbool.h"

struct memory_segment {
	uint64_t 				memory_len;
	struct memory_segment* 	next_seg;
	struct memory_segment* 	prev_seg;
	struct memory_segment* 	next_free_seg;
	struct memory_segment* 	prev_free_seg;
	bool 					free;
} memory_segment_t;


#endif
