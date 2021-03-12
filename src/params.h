#pragma once

#define BLOCK_SIZE 1024
#define INODE_SIZE 128
#define N_SECTORS 512
#define N_INODES 512
#define FILENAME_LENGTH 31
#define DIRECT_POINTERS 2
#define INDIRECT_BLOCK_SIZE 2

#define SUPERBLOCK_OFFSET 0
/* решил не хранить bitmap-ы, а использовать информацию из superblock
#define BITMAP_SECTORS_OFFSET BLOCK_SIZE
#define BITMAP_INODE_OFFSET (2 * BLOCK_SIZE)
*/
#define INODES_OFFSET (BLOCK_SIZE)
#define SECTORS_OFFSET (INODES_OFFSET + N_INODES * INODE_SIZE)

#define MAX_FILESIZE ((DIRECT_POINTERS + INDIRECT_BLOCK_SIZE) * BLOCK_SIZE)

#define MAGIC_NUMBER 826
#define FS_NAME "minifs"
