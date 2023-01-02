#include "mmap.h"
#include <stdint.h>

static multiboot_memory_map_t mmap[10] = {0};
static uint8_t entries_count = 0;

void extract_mmap(unsigned long mbi_addr)
{
	struct multiboot_tag *tag;
	unsigned size;

	size = *(unsigned *) mbi_addr;

	 for (tag = (struct multiboot_tag *) (mbi_addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
                                       + ((tag->size + 7) & ~7)))
    {
		if (tag->type == MULTIBOOT_TAG_TYPE_MMAP)
		{
			multiboot_memory_map_t *mmap_entery;

			for (mmap_entery = ((struct multiboot_tag_mmap *) tag)->entries;
                 (multiboot_uint8_t *) mmap_entery 
                   < (multiboot_uint8_t *) tag + tag->size;
                 mmap_entery = (multiboot_memory_map_t *) 
                   ((unsigned long) mmap_entery
                    + ((struct multiboot_tag_mmap *) tag)->entry_size))
              	if (mmap_entery->type == MULTIBOOT_MEMORY_AVAILABLE)
					mmap[entries_count++] = *mmap_entery; 
		}
	}
}

multiboot_memory_map_t* get_mmap_entery(int index)
{
	if (entries_count <= index)
		return 0;
	return mmap + index;
}
