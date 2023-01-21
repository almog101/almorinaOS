#pragma once
#include <stdint.h>
#include "bitset.h"

#define BLOCK_SIZE 4096

#define NO_SPACE_LEFT 0
#define SAVED_DATA_PARTLY 1
#define SAVED_DATA_SICCESSFULLY 2

enum INODE_TYPE {
	INODE_TYPE_FILE,
	INODE_TYPE_DIR,
	INODE_TYPE_LINK,
};

typedef struct
{
	uint32_t	inodes_count;		/* Inodes count 	*/
	uint32_t	blocks_count;		/* Blocks count 	*/
	uint32_t	first_inode;		/* First inode 		*/
	uint32_t	first_data_block;	/* First Data Block	*/
	uint32_t	block_size;			/* Block size 		*/
	bitset_t	inodes_bitset;		/* Inodes Bitset 	*/
	bitset_t	blocks_bitset;		/* Blocks Bitset 	*/
} fs_superblock_t;

typedef struct
{
    uint8_t		mode;		/* File mode 				*/
    uint32_t	size;		/* Size in bytes 			*/
    uint32_t	atime;		/* Access time 				*/
    uint32_t	ctime;		/* Creation time 			*/
    uint32_t 	mtime;		/* Modification time 		*/
    uint32_t	blocks[15];	/* Pointers to blocks 		*/
							/* 12 direct pointers		*/
							/* +3 indirect pointers		*/
} fs_inode_t;

void* fs_initialize(int inodes_count, int blocks_count);
fs_inode_t* fs_create_inode(fs_superblock_t* device, uint8_t type);

int fs_add_block(fs_superblock_t* device, fs_inode_t* inode, char* data);
int fs_change_block(fs_superblock_t* device, fs_inode_t* inode, char* new_data);

/*
super-block:
	list of inodes
	inode:
		metadata:
			general information about a file/directory
		list of blocks [that belong to 1 file/diractory]
		blocks:
			parts of a disc [memory]
			are THE CONTENT
		indirect blocks:
			point to a block of block pointers [??]
*/