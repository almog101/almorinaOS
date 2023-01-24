#pragma once
#include <stdint.h>
#include "bitset.h"

#define BLOCK_SIZE 4096
#define NUM_OF_BLOCKS_IN_INODE 15

#define NO_SPACE_LEFT 0
#define SAVED_DATA_PARTLY 1
#define SAVED_DATA_SICCESSFULLY 2

#define FS_MAX_FILENAME_SIZE (128-(sizeof(int)) - (sizeof(uint8_t)))

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
    uint32_t	blocks[NUM_OF_BLOCKS_IN_INODE];	
							/* Pointers to blocks 		*/
							/* 12 direct pointers		*/
							/* +3 indirect pointers		*/
} fs_inode_t;

typedef struct {
	fs_inode_t* inode;
	char name[FS_MAX_FILENAME_SIZE];
	uint8_t is_taken;
} fs_dir_entry;

void* fs_initialize(int inodes_count, int blocks_count);
fs_inode_t* fs_create_inode(fs_superblock_t* device, uint8_t type);
void* fs_create_block(fs_superblock_t* device);
int fs_inode_write_data(fs_superblock_t* device, fs_inode_t* inode, char* data);
char* fs_inode_get_data(fs_superblock_t* device, fs_inode_t* inode);
fs_inode_t* fs_dir_add_entry(fs_superblock_t* device, fs_inode_t* dir,  char* filename, uint8_t type);

extern fs_superblock_t* ramfs_device;
extern fs_inode_t* ramfs_root;
