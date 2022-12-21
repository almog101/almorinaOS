#include "heap.h"
#include "stdbool.h"

static memory_segment_t* first_free_memory_seg;

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

void split_free_seg(memory_segment_t* free_seg, uint64_t size)
{
	memory_segment_t* new_seg = 
		(memory_segment_t*)((uint64_t)free_seg + sizeof(memory_segment_t) + size);

	new_seg->free = true;
	new_seg->len = ((uint64_t)free_seg->len) - (sizeof(memory_segment_t) + size);

	new_seg->next_free_seg = free_seg->next_free_seg;
	new_seg->next_seg = free_seg->next_seg;
	new_seg->prev_seg = free_seg;
	new_seg->prev_free_seg = free_seg->prev_free_seg;

	free_seg->next_free_seg = new_seg;
	free_seg->next_seg = new_seg;
	free_seg->len = size;
}


void* malloc(uint64_t size)
{
	// align allocated size to be multiple of 8
	uint64_t reminder = size % 8;
	size -= reminder;
	if (reminder != 0) size+=8;

	memory_segment_t* curr_seg = first_free_memory_seg;
	do
	{
		if (curr_seg->len < size) // current segment is to small
		{
			curr_seg = curr_seg->next_free_seg;
			continue;
		}

		if (curr_seg->len > size + sizeof(memory_segment_t)) // is free segment big enough to split
			split_free_seg(curr_seg, size);

		if(curr_seg == first_free_memory_seg)
			first_free_memory_seg = curr_seg->next_free_seg;
		curr_seg->free = false;

		if (curr_seg->prev_free_seg != 0)
			curr_seg->prev_free_seg->next_free_seg = curr_seg->next_free_seg;

		if (curr_seg->next_free_seg != 0)
			curr_seg->next_free_seg->prev_free_seg = curr_seg->prev_free_seg;

		if (curr_seg->prev_seg != 0)
			curr_seg->prev_seg->next_free_seg = curr_seg->next_free_seg;

		if (curr_seg->next_seg != 0)
			curr_seg->next_seg->prev_free_seg = curr_seg->prev_free_seg;

		return curr_seg + 1;
	} while(curr_seg != 0);

	return 0; // no free space is left / heap is full
}
