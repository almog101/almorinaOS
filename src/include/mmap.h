#ifndef MMAP
#define MMAP
#include <multiboot2.h>

void extract_mmap(unsigned long mbi_addr);
multiboot_memory_map_t* get_mmap_entery(int index);

#endif

