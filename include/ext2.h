

/*********************************
 * Reviewer: 				
 * Author: Yohai Azoulai			
 * File: ext2.c					
 * ******************************/

#ifndef __EXT2_H__
#define __EXT2_H__


#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <ext2fs/ext2_fs.h>
#include <stdio.h>
#include <sys/stat.h> /*ISDIR*/
#include <string.h>
#include <stdlib.h>

#define _XOPEN_SOURCE 501
#define _POSIX_C_SOURCE  200809L
#define BLOCK_SIZE  4096
#define SUPERBLOCK_OFFSET       1024
#define SUPERBLOCK_SIZE         1024
#define BLOCK_OFFSET(block) (BLOCK_SIZE * (block))

typedef struct ext2_super_block    super_t;
typedef struct ext2_group_desc     group_t;
typedef struct ext2_inode          inode_t;
typedef struct ext2_dir_entry    dir_entry_t;

void ReadGroupDesc(int fd_to_use ,group_t * buffer);

int ReadMetatoBuffer(super_t * buffer);

void ReadRootFolder(int fd,group_t * group_descriptor, inode_t * inode);

void ReadBlock(int fd,inode_t inode, dir_entry_t dir, size_t size, int block);

#endif 

