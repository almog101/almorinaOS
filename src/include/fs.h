#pragma once
#include <stdint.h>
#include "bitset.h"

#define BLOCK_SIZE 4096

enum INODE_TYPE {
	INODE_TYPE_FILE,
	INODE_TYPE_DIR,
	INODE_TYPE_LINK,
};

typedef struct
{
	uint32_t	inodes_count;		/* Inodes count */
	uint32_t	blocks_count;		/* Blocks count */
	uint32_t	first_inode;		/* First inode */
	uint32_t	first_data_block;	/* First Data Block */
	uint32_t	block_size;		/* Block size */
	bitset_t	inodes_bitset;		/* Inodes Bitset */
	bitset_t	blocks_bitset;		/* Blocks Bitset */
} fs_superblock_t;


typedef struct
{
    uint8_t			mode;		/* File mode */
    uint32_t			size;		/* Size in bytes */
    uint32_t			atime;		/* Access time */
    uint32_t			ctime;		/* Creation time */
    uint32_t 			mtime;		/* Modification time */
    uint32_t			block[15];	/* Pointers to blocks */
} fs_inode_t;

void* fs_initialize(int inodes_count, int blocks_count);
fs_inode_t* fs_create_inode(fs_superblock_t* device);
