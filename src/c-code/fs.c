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
		if (BITSET_READBIT(device->inodes_bitset, i) == 0)
			break;
	
	if (i == device->inodes_count)
		return 0;

	BITSET_SETBIT(device->inodes_bitset, i, 1);
	fs_inode_t* inode = &((fs_inode_t*)device->first_inode)[i];
	*inode = (fs_inode_t){0};
	inode->mode = type;

	return inode;
}
