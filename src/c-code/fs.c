#include "fs.h"
#include "bitset.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

fs_superblock_t* ramfs_device;
fs_inode_t* ramfs_root;

/**
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

/**
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

	// initialize all the blocks in the created inode with NULL
	for (int i =0; i<sizeof(inode->blocks)/sizeof(inode->blocks[0]); i++)
		inode->blocks[i] = 0;

	return inode;
}

/**
returns adress of available block
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

	/*
	the following lines:
		- mark the found available block as taken
		- save the address of available block
		- clear the block's data
	*/
	BITSET_SETBIT(device->blocks_bitset, block_bitset_index, 1);
	void* block = (void*)(device->first_data_block + block_bitset_index*BLOCK_SIZE);
	memset(block, 0, BLOCK_SIZE);

	return block;
}

/**
saves the given data on dist (on device) & links an inode to it
INPUT:	
- super-block with all the inodes and blocks [our file system]
- inode to link the data [the block] to
- string to save
OUTPUT:
- none
*/
void fs_inode_write_data(fs_superblock_t* device, fs_inode_t* inode, char* data)
{
	int size = strlen(data);
	int block_index = 0;

	// loop over blocks of inode
	for (block_index = 0; block_index < NUM_OF_BLOCKS_IN_INODE; block_index++)
	{
		// if needed, allocate new block
		if (inode->blocks[block_index] == 0)
			inode->blocks[block_index] = fs_create_block(device);

		// clear the block's data
		memset(inode->blocks[block_index], 0, BLOCK_SIZE);

		// devide the data to small chunks
		uint16_t copy_size = ((size < BLOCK_SIZE) ? size : BLOCK_SIZE);
		// copy the data to the block
		strncpy(inode->blocks[block_index], data, copy_size);
		
		// check if last chunk was enough to save all the data
		if (size < BLOCK_SIZE)
			break;
		size -= BLOCK_SIZE;
	}
	inode->size = size;
}

/**
returns the directory entry of the given file [if so exists]
INPUT:
- directory to search the given file in
- file's name to search
OUTPUT:
- result of the search of the given file in the given directory
*/
fs_inode_t* fs_get_inode_by_filename(fs_inode_t* dir, const char* filename)
{
	for (int i = 0; i < NUM_OF_BLOCKS_IN_INODE; i++)
	{
		// check if current block is empty
		if (dir->blocks[i] == 0)
			continue;

		// go over block's content
		for (int j = 0; j<BLOCK_SIZE; j+=sizeof(fs_dir_entry))
		{
			fs_dir_entry* ent = dir->blocks[i] + j;
			// check if the given file is in the given inode
			if (strcmp(ent->name, filename) == 0)
				return ent->inode;
		}
	}

	// no file was found
	return 0;
}

/**
TODO: add description plz
INPUT:
-
-
-
OUTPUT:
- 
*/
fs_inode_t* fs_get_inode_dir(fs_superblock_t* device, fs_inode_t* dir, const char* path)
{
	char* filename_start = path;
	char* filename_end = path;

	// check if path starts with '/' in order to "remove" it
	if (path[0] == '/')
	{
		dir = ramfs_root;
		filename_start++;
		filename_end++;
	}

	char filename[FS_MAX_FILENAME_SIZE + 1] = {0};
	fs_inode_t* ent = 0;

	/// TODO: add there comment plz
	while(*filename_end)
	{
		if (*filename_end == '/')
		{
			if (filename_end - filename_start > 1)
			{
				// get the name of the file given in path
				strncpy(filename, filename_start, filename_end-filename_start);
				filename[filename_end-filename_start] = 0;

				ent = fs_get_inode_by_filename(dir, filename);
				// check if the gotten directory entry is not empty & is directory
				if (ent && ent->mode == INODE_TYPE_DIR)
					dir = ent;
				else
					// no directory was found
					return 0;
			}
			filename_start=filename_end+1;
		}
		filename_end++;
	}

	// check if the needed directory was found
	if (ent == 0)
		// the needed directory is the given directory
		return dir;
	
	/// TODO: there too
	return ent;
}

/**
returns a file's name from the given path & saves its length in 'len'
INPUT:
- path to search the file from
- int pointer to save the file's length
OUTPUT:
- file's name
*/
char* fs_extract_filename_from_path(const char* path, int* len)
{
	// gets the pointer to the end of the given file
	char* filename_end = path;
	while(*(filename_end++)); 
	while(*(--filename_end) == '/');

	// gets the pointer to the start of the given file
	char* filename_start = filename_end-1;
	while(*(filename_start) && *(filename_start) != '/')
		filename_start--;
	filename_start++;

	// save the file's name's length & return the name
	*len = filename_end-filename_start;
	return filename_start;
}

/**
TODO: ??
INPUT:
-
-
-
OUTPUT:
- 
*/
fs_inode_t* fs_get_inode(fs_superblock_t* device, fs_inode_t* dir, const char* path)
{
	fs_inode_t* parent_dir  = fs_get_inode_dir(device, dir, path);
	int len = 0;
	char filename[FS_MAX_FILENAME_SIZE + 1] = {0};

	strncpy(filename, fs_extract_filename_from_path(path, &len), len);
	filename[len] = 0;

	return fs_get_inode_by_filename(parent_dir, filename);
}

/**
returns a string of the data saved in the given inode of the given device
INPUT:
- device where the inode is placed in
- inode to get the data from
OUTPUT:
- string of inode's data
*/
char* fs_inode_get_data(fs_superblock_t* device, fs_inode_t* inode) // change
{
	int block_index = 0;
	char* data = 0;
	int data_size = 0;

	// loop over blocks of inode
	for (block_index = 0; block_index < NUM_OF_BLOCKS_IN_INODE; block_index++)
	{
		// check if a block is empty in order to add to 'size' only filled blocks
		if(inode->blocks[block_index] != 0)
			data_size += strlen((char*)inode->blocks[block_index]);
	}
	data = (char*)malloc(data_size + 1);

	// loop over blocks of inode
	for (block_index = 0; block_index < NUM_OF_BLOCKS_IN_INODE; block_index++)
	{
		// check if a block is empty in order to add its content to 'data' if it is not
		if(inode->blocks[block_index] != 0)
		{
			int block_ln = strlen((char*)inode->blocks[block_index]);
			strncpy(data, inode->blocks[block_index], block_ln);

		}
	}
	data[data_size] = 0;

	return data;
}

/**
adds the given directory to the given device
INPUT:
- device to save the directory on
- directory to save
- directory's name
- directory's type
OUTPUT:
- inode of the saved directory
*/
fs_inode_t* fs_dir_add_entry(fs_superblock_t* device, fs_inode_t* dir, char* filename, uint8_t type)
{
	if (dir->mode != INODE_TYPE_DIR)
		return 0;

	int filename_size = strlen(filename);

	// loops over all blocks & directory entries in order to find a free entry
	/// TODO: a little bit more comments would be nice
	for (int i =0; i<NUM_OF_BLOCKS_IN_INODE; i++)
	{
		if (dir->blocks[i] == 0)
			dir->blocks[i] = fs_create_block(device);

		for (int j= 0; j<BLOCK_SIZE; j+=sizeof(fs_dir_entry))
		{
			fs_dir_entry* ent = dir->blocks[i] + j;
			if (strcmp(ent->name, filename) == 0)
				return;

			if (ent->is_taken == 0)
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
