#include "mmap.h"
#include "heap.h"
#include "stdbool.h"

static memory_segment_t *first_free_memory_seg;

int initialize_memory(unsigned long magic, unsigned long addr)
{
	extract_mmap(addr);
	multiboot_memory_map_t* ent = get_mmap_entery(0);
	initialize_heap(ent->addr, ent->len);
	printf("Heap initialized. address: 0x%x, length: 0x%x\n", ent->addr, ent->len);
}

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

void split_free_seg(memory_segment_t *free_seg, uint64_t size)
{
	memory_segment_t *new_seg = (memory_segment_t*)((uint64_t)free_seg + sizeof(memory_segment_t) + size);

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
	uint64_t remainder = size % 8;
	size -= remainder;
	if (remainder != 0) size += 8;

	memory_segment_t* curr_seg = first_free_memory_seg;

	do
	{
		if (curr_seg->len < size) // current segment is too small
		{
			curr_seg = curr_seg->next_free_seg;
			continue;
		}

		if (curr_seg->len > size + sizeof(memory_segment_t)) // is free segment big enough to split
			split_free_seg(curr_seg, size);

		if (curr_seg == first_free_memory_seg)
			first_free_memory_seg = curr_seg->next_free_seg;
		
		curr_seg->free = false;
		// curr_seg->len = size;

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

/*
when a large block of memory is tried to be allocated [my English is great, I know]
our malloc function skips over small memory blocks that we might want to use later
so this function is needed to combine those small blocks of free memory
with another free blocks of memory if they are right next to them
*/
void combine_segments(memory_segment_t *f, memory_segment_t *s)
{
	if((f == 0) || (s == 0))
		return;

	if(f < s)
	{
		f->len += s->len + sizeof(memory_segment_t);
		f->next_seg = s->next_seg;
		f->next_free_seg = s->next_free_seg;
		s->next_seg->prev_seg = f;
		s->next_seg->prev_free_seg = f;
		s->next_free_seg->prev_free_seg = f;
	}

	else
	{
		s->len += f->len + sizeof(memory_segment_t);
		s->next_seg = f->next_seg;
		s->next_free_seg = f->next_free_seg;
		f->next_seg->prev_seg = s;
		f->next_seg->prev_free_seg = s;
		f->next_free_seg->prev_free_seg = s;
	}
}

void free(void* address)
{
	// get the correct address of the memory segment to free
	memory_segment_t *curr_memory_seg = ((memory_segment_t*)address) - 1;
	curr_memory_seg->free = true;

	if (curr_memory_seg < first_free_memory_seg)
		first_free_memory_seg = curr_memory_seg;

	if (curr_memory_seg->next_free_seg != 0)
	{
		if (curr_memory_seg->next_free_seg->prev_free_seg < curr_memory_seg)
			curr_memory_seg->next_free_seg->prev_free_seg = curr_memory_seg;
	}

	if (curr_memory_seg->prev_free_seg != 0)
	{
		if(curr_memory_seg->prev_free_seg->next_free_seg > curr_memory_seg)
			curr_memory_seg->prev_free_seg->next_free_seg = curr_memory_seg;
	}

	if (curr_memory_seg->next_seg != 0)
	{
		curr_memory_seg->next_seg->prev_seg = curr_memory_seg;
		if (curr_memory_seg->next_seg->free)
			combine_segments(curr_memory_seg, curr_memory_seg->next_seg);
	}

	if (curr_memory_seg->prev_seg != 0)
	{
		curr_memory_seg->prev_seg->next_seg = curr_memory_seg;
		if (curr_memory_seg->prev_seg->free)
			combine_segments(curr_memory_seg, curr_memory_seg->prev_seg);
	}
}
