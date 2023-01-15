#pragma once
#include <stdint.h>

enum INODE_TYPE {
	INODE_TYPE_FILE,
	INODE_TYPE_DIR,
	INODE_TYPE_LINK,
};

typedef struct
{
	uint32_t	inodes_count;         /* Inodes count */
	uint32_t	blocks_count;         /* Blocks count */
	uint32_t	first_data_block;     /* First Data Block */
	uint32_t	log_block_size;       /* Block size */
	uint32_t	inode_size;			 /* Inodes size */
	uint32_t	first_inode;			 /* First inode */
} fs_superblock_t;


typedef struct
{
    enum INODE_TYPE		mode;         /* File mode */
    uint32_t			size;         /* Size in bytes */
    uint32_t			atime;        /* Access time */
    uint32_t			ctime;        /* Creation time */
    uint32_t 			mtime;        /* Modification time */
    uint32_t			block[15];	/* Pointers to blocks */
} fs_inode_t;
