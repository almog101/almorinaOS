#include "fs.h"
#include "bitset.h"
#include <stdint.h>

void* fs_initialize(int inodes_count, int blocks_count)
{
	int size = sizeof(fs_superblock_t) + BITSET_SIZE(inodes_count) + BITSET_SIZE(blocks_count)  + sizeof(fs_inode_t)*inodes_count + BLOCK_SIZE*blocks_count;
	fs_superblock_t* superblock = malloc(size);	
	
	superblock->inodes_count = inodes_count;
	superblock->blocks_count = blocks_count;
	superblock->block_size = BLOCK_SIZE;
	superblock->first_inode = superblock + sizeof(fs_superblock_t) + BITSET_SIZE(inodes_count) + BITSET_SIZE(blocks_count);
	superblock->first_data_block = superblock->first_inode + inodes_count*sizeof(fs_inode_t);

	superblock->inodes_bitset = superblock + sizeof(fs_superblock_t);
	superblock->blocks_bitset = superblock + sizeof(fs_superblock_t) + BITSET_SIZE(inodes_count);

	return superblock;
}

fs_inode_t* fs_create_inode(fs_superblock_t* device, uint8_t type)
{
	int i = 0;
	for (i = 0; i<device->inodes_count; i++)
	{
		if (BITSET_READBIT(device->inodes_bitset, i) == 0)
			break;
	}
	
	if (i == device->inodes_count)
		return 0;

	BITSET_SETBIT(device->inodes_bitset, i, 1);
	fs_inode_t* inode = &((fs_inode_t*)device->first_inode)[i];
	*inode = (fs_inode_t){0};
	inode->mode = type;
	inode->block[0] = NULL;

	return inode;
}

bool ckeck_indirect_pointers(int i, uint32_t* pointer_to_blocks, uint32_t block_to_add)
{
	if(i == NULL)
	{
		pointer_to_blocks[i] = malloc(uint32_t * 15);
		pointer_to_blocks[i][0] = block_to_add;
		pointer_to_blocks[i + 1] = NULL;
		pointer_to_blocks[i][1] = NULL;
		return 1;
	}
	else
	{
		for(int j = 0; j < 15; j++)
		{
			if(pointer_to_blocks[i][j] == NULL)
			{
				pointer_to_blocks[i][j] = block_to_add;
				if((j + 1) != 15)
					pointer_to_blocks[i][j + 1] = NULL;
				return 1;
			}
		}
	}
	return 0;
}

void fill_blocks(uint32_t* pointer_to_blocks, uint32_t block_to_add)
{
	int i = 0;

	for(i = 0; i < 12; i++)
	{
		if(pointer_to_blocks[i] == NULL)
		{
			pointer_to_blocks[i] = block_to_add;
			pointer_to_blocks[i + 1] = NULL;
			return;
		}
	}

	if(ckeck_indirect_pointers(i, pointer_to_blocks, block_to_add))
		return;

	if(ckeck_indirect_pointers(++i, pointer_to_blocks, block_to_add))
		return;

	if(++i == NULL)
	{
		pointer_to_blocks[i] = malloc(uint32_t * 15);
		pointer_to_blocks[i][0] = block_to_add;
		pointer_to_blocks[i + 1] = NULL;
		pointer_to_blocks[i][1] = NULL;
		return;
	}
	else
	{
		fill_blocks(pointer_to_blocks[i], block_to_add);
	}
}

void fs_add_block(fs_inode_t* inode, uint32_t block_pointer) /* probably should be uint32_t pointer [not sure]*/
{
	fill_blocks(&(inode->block[0]), block_pointer); // should check if works
}