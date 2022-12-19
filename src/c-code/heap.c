#include "heap.h"
#include "stdbool.h"

memory_segment_t* first_free_memory_seg;

void initialize_heap(uint64_t addr, uint64_t size)
{
	first_free_memory_seg = (memory_segment_t*)addr;
	first_free_memory_seg->len = size - sizeof(memory_segment_t);
	first_free_memory_seg->next_seg = 0;
	first_free_memory_seg->prev_seg = 0;
	first_free_memory_seg->next_free_seg = 0;
	first_free_memory_seg->prev_free_seg = 0;
	first_free_memory_seg->free = true;
}

void* malloc(uint64_t size)
{
	memory_segment_t* curr_seg = first_free_memory_seg;
	while (true)
	{
		if (curr_seg->len >= size)
			return curr_seg + 1;
	}
}
