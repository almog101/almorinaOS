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
- saves the given data on dist (on device)
- marks it as saved on device
- links the saved data [block] to inode
INPUT:	
- super-block with all the inodes and blocks [our file system]
- inode to link the data [the block] to
- string to save
OUTPUT:
- result of the attempt to save the given data
*/
int fs_add_block(fs_superblock_t* device, fs_inode_t* inode, char* data)
{
	int i = 0, j = 0;

	// search for available space in blocks_bitset [available index]
	for (i = 0; i<device->blocks_count; i++)
	{
		if (BITSET_READBIT(device->blocks_bitset, i) == 0)
			break;
	}
	
	// check if the index is out of range [if there are any available blocks left]
	if (i == device->blocks_count)
		return NO_SPACE_LEFT;
	
	/*
	check if:
		- the device ran out of available blocks
		- the data's size is biger than one block's size
		- the inode ran out of available pointers to blocks
	*/
	while(i != device->blocks_count && strlen(data) > BLOCK_SIZE && j < NUM_OF_BLOCKS_IN_INODE)
	{
		/*
		the following lines:
			- mark the bitset of used block as used
			- save the data to a block
			- shorten the data [in order to save it in nother block the next loop]
		*/
		BITSET_SETBIT(device->blocks_bitset, i, 1); // set bitset to 1
		char* block = &((char*)device->first_data_block)[i++]; // get address for saving the data
		inode->blocks[j++] = block; // save the address
		strncpy(block, data, BLOCK_SIZE); // save the data [of 1 block size]
		data = &data[BLOCK_SIZE]; // shorten the data [delete the data that was already saved]

		// make sure the bitset of next block is available and if the device ran out of available blocks
		// if not available, pass to the next block
		while(i != device->blocks_count && BITSET_READBIT(device->blocks_bitset, i) != 0)
			i++;
	}

	// at this point the size of data is smaller than the size of 1 block
	// so we make sure the device & the inode did not run out of available blocks and we save the data
	if(i != device->blocks_count && j < NUM_OF_BLOCKS_IN_INODE)
	{
		BITSET_SETBIT(device->blocks_bitset, i, 1);
		char* block = &((char*)device->first_data_block)[i];
		inode->blocks[j] = block;
		strncpy(block, data, strlen(data));
		inode->size += strlen(data);
	}
	// notify that only part of the given data was saved and that the device ran out of available blocks
	else
		return SAVED_DATA_PARTLY;

	// the given data was saved and linked to the given inode successfully 
	return SAVED_DATA_SICCESSFULLY;
}

/*
- frees the previous saved data in the given inode [and frees it from the given device]
- saves the given data in the given inode [and device] using the function fs_add_block
INPUT:
- super-block with all the inodes and blocks [our file system]
- inode to link the data [the block] to
- string to save
OUTPUT:
- result of the attempt to save the given data
*/
int fs_change_block(fs_superblock_t* device, fs_inode_t* inode, char* new_data)
{
	int i = 0, block_index = 0;

	// go over all the blocks in the inode
	for(i = 0; i < NUM_OF_BLOCKS_IN_INODE; i++)
	{
		// check if the block is empty
		if(inode->blocks[i] == 0)
			break;
		
		// the following lines clear the data from the inode and from the device
		block_index = (&(inode->blocks[i]) - &(inode->blocks[0])) / BLOCK_SIZE;
		BITSET_SETBIT(device->blocks_bitset, block_index, 0);
		memset((void*)inode->blocks[i], 0, BLOCK_SIZE);
		inode->blocks[i] = 0;
	}

	// add the new data after freeing the space for it
	return fs_add_block(device, inode, new_data);
}
