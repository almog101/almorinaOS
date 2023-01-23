#include "fs.h"
#include "bitset.h"
#include <stdint.h>
#include <string.h>

/*
initializes out file system with the given number if inodes & blocks
INPUT:
- number of inodes to create
- number of blocks to create
OUTPUT:
- address of the created super-block
*/
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

/*
creates inode, saves it in the given device & returns the saved inode
INPUT:	
- super-block with all the inodes and blocks [our file system]
- type of the inode to create
OUTPUT:
- address to the created inode
*/
fs_inode_t* fs_create_inode(fs_superblock_t* device, uint8_t type)
{
	int i = 0;

	// search for available space in inodes_bitset [available index]
	for (i = 0; i<device->inodes_count; i++)
	{
		if (BITSET_READBIT(device->inodes_bitset, i) == 0)
			break;
	}
	
	// check if the index is out of range [if there are any available inodes left]
	if (i == device->inodes_count)
		return 0;

	
	// the following lines create inode with suitable parameters [including type] and save in on the device
	BITSET_SETBIT(device->inodes_bitset, i, 1);
	fs_inode_t* inode = &((fs_inode_t*)device->first_inode)[i];
	*inode = (fs_inode_t){0};
	inode->mode = type;

	for (int i =0; i<sizeof(inode->blocks)/sizeof(inode->blocks[0]); i++)
		inode->blocks[i] = 0;

	return inode;
}

/*
creates block and returns it
INPUT:	
- super-block with all the inodes and blocks [our file system]
OUTPUT:
- allocated block
*/
void* fs_create_block(fs_superblock_t* device)
{
	int block_bitset_index = -1;
	// search for available space in blocks_bitset [available index]
	for (int i = 0; i<device->blocks_count; i++)
	{
		if (BITSET_READBIT(device->blocks_bitset, i) == 0)
		{
			block_bitset_index = i;
			break;
		}
	}
	
	// check if the index is out of range [if there are any available blocks left]
	if (block_bitset_index == -1)
		return 0;

	BITSET_SETBIT(device->blocks_bitset, block_bitset_index, 1);
	void* block = (void*)(device->first_data_block + block_bitset_index*BLOCK_SIZE);
	memset(block, 0, BLOCK_SIZE);

	return block;
}

/*
- saves the given data on dist (on device)
INPUT:	
- super-block with all the inodes and blocks [our file system]
- inode to link the data [the block] to
- string to save
OUTPUT:
- result of the attempt to save the given data
*/
int fs_inode_write_data(fs_superblock_t* device, fs_inode_t* inode, char* data)
{
	int size = strlen(data);
	int block_index = 0;

	// loop over blocks of inode
	for (block_index = 0; block_index<NUM_OF_BLOCKS_IN_INODE; block_index++)
	{
		// if needed, allocate new block
		if (inode->blocks[block_index] == 0)
			inode->blocks[block_index] = fs_create_block(device);

		// clear the block data
		memset(inode->blocks[block_index], 0, BLOCK_SIZE);

		// devide the data to small chunks
		uint16_t copy_size = ((size < BLOCK_SIZE) ? size : BLOCK_SIZE);
		// copy the data to the block
		strncpy(inode->blocks[block_index], data, copy_size);
		
		// if last chunk break
		if (size < BLOCK_SIZE)
			break;
		size -= BLOCK_SIZE;
	}
	inode->size = size;

	return 0;
}

fs_inode_t* fs_dir_add_entry(fs_superblock_t* device, fs_inode_t* dir,  char* filename, uint8_t type)
{
	if (dir->mode != INODE_TYPE_DIR)
		return;

	int filename_size = strlen(filename);

	// loops over all blocks, and dir entries abd finds
	// a free entry
	for (int i =0; i<NUM_OF_BLOCKS_IN_INODE; i++)
	{
		if (dir->blocks[i] == 0)
			dir->blocks[i] = fs_create_block(device);

		for (int j= 0; j<BLOCK_SIZE; j+=sizeof(fs_dir_entry))
		{
			fs_dir_entry* ent = dir->blocks[i] + j;
			if (strcmp(ent->name, filename) == 0)
				return;

			if ( ent->is_taken == 0 )
			{
				ent->is_taken = 1;
				ent->inode = fs_create_inode(device, type);
				strncpy(ent->name, filename, \
				(filename_size < FS_MAX_FILENAME_SIZE) ? filename_size : FS_MAX_FILENAME_SIZE);
				return ent->inode;
			}
		}
	}

	return 0;
}
