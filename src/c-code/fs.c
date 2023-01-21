#include "fs.h"
#include "bitset.h"
#include <stdint.h>
#include <string.h>

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

	return inode;
}

int fs_add_block(fs_superblock_t* device, fs_inode_t* inode, char* data)
{
	int i = 0, j = 0;
	for (i = 0; i<device->blocks_count; i++)
	{
		if (BITSET_READBIT(device->blocks_bitset, i) == 0)
			break;
	}
	
	if (i == device->blocks_count)
		return NO_SPACE_LEFT;
	
	while((strlen(data) * sizeof(char)) > BLOCK_SIZE && i != device->blocks_count)
	{

		BITSET_SETBIT(device->blocks_bitset, i, 1); // set the bitset of used blocl to 1
		char* block = &((char*)device->first_data_block)[i++]; // get the address of where to save the data
		inode->blocks[j++] = block;
		strncpy(block, data, (BLOCK_SIZE / sizeof(char))); // save the data to the block
		data = &data[BLOCK_SIZE / sizeof(char)]; // shorten the string
	}

	if(i != device->blocks_count)
	{
		BITSET_SETBIT(device->blocks_bitset, i, 1);
		char* block = &((char*)device->first_data_block)[i];
		inode->blocks[j] = block;
		strncpy(block, data, strlen(data));
	}
	else
		return SAVED_DATA_PARTLY;

	return SAVED_DATA_SICCESSFULLY;
}
