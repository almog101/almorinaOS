#include "fs.h"

void* initialize_ramfs(int inodes_count, int blocks_count)
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
