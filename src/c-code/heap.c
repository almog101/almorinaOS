#include "mmap.h"
#include "heap.h"
#include "stdbool.h"

mem_segment_t* heap = 0;
mem_segment_t* free_seg = 0;

int initialize_memory(unsigned long magic, unsigned long addr)
{
	extract_mmap(addr);
	multiboot_memory_map_t* ent = get_mmap_entery(0);
	initialize_heap(ent->addr, ent->len);
	printf("Heap initialized. address: 0x%x, length: 0x%x\n", ent->addr, ent->len);
}


void initialize_heap(uint64_t addr, uint64_t size)
{
	heap = (mem_segment_t*)addr;
	free_seg = heap;

    free_seg->len = size - sizeof(mem_segment_t);
    free_seg->free = 1;
    free_seg->next = 0;
}


void split(mem_segment_t * fitting_slot, uint64_t size) {
    mem_segment_t * new = (void * )((void * ) fitting_slot + size + sizeof(mem_segment_t));
    new->len = (fitting_slot->len) - size - sizeof(mem_segment_t);
    new->free = 1;
    new->next = fitting_slot->next;
    fitting_slot->len = size;
    fitting_slot->free = 0;
    fitting_slot->next = new;
}

void * malloc(uint64_t noOfBytes) 
{
    mem_segment_t * curr, * prev;
    void * result;

    curr = free_seg;
    while ((((curr->len) < noOfBytes) || ((curr->free) == 0)) && (curr->next != 0)) {
        prev = curr;
        curr = curr->next;
    }

    if (curr->len == noOfBytes) 
	{
        curr->free = 0;
        result = (void * )(++curr);
    } 
	else if (curr->len > noOfBytes + sizeof(mem_segment_t)) 
	{
        split(curr, noOfBytes);
        result = (void * )(++curr);
    } 
	else 
	{
        result = 0;
        printf("Sorry. No sufficient memory to allocate\n");
		printf("%d %d\n", free_seg, curr);
		print_segs();
    }
    return result;
}

void merge() {
    mem_segment_t * curr;
    curr = free_seg;

    do
	{
        if (curr->free && curr->next->free) 
		{
            curr->len += (curr->next->len) + sizeof(mem_segment_t);
            curr->next = curr->next->next;
        }
		else
			curr = curr->next;
    }while (curr && curr->next);
}

void free(void * ptr) 
{
	mem_segment_t * curr = heap;
	bool isvalid = false;

	// finds if the address to free is an actual segment
	do
	{
		if (((void*)curr) + sizeof(mem_segment_t) == ptr)
		{
			isvalid = true;
			break;
		}
		curr = curr->next;
	}while(curr);
	
	if (!isvalid)
	{
		printf("Please provide a valid pointer allocated by malloc. %d\n", ptr);
		return;
	}

	curr->free = 1;
	merge();
}

void print_segs() {
    mem_segment_t* curr =heap;
	puts("----- Mem Segs -----\n");
    do{
        printf("0x%x %d %d\n", curr, curr -> len, curr -> free);
        curr = curr -> next;
    }while (curr);
	puts("--------------------\n");
}
