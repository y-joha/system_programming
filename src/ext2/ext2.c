#include <stdio.h> /* for prints */
#include <sys/stat.h> /* Block Structs */
#include <fcntl.h> /* Block Structs */
#include <unistd.h> /* read */
#include <linux/types.h>
#include <linux/magic.h>
#include "ext2.h"

int ReadMetatoBuffer(super_t * buffer)
{
    int ram0_fd = 0;
    
    ram0_fd = open("/dev/ram0",O_RDONLY);
    /*The first 1024 bytes of the disk, the "boot block"
    
    the following is the BlockGroup 0 and within it are
    superblock -> size of super block, how many group discritors
    GroupDiscriptors -> Data Block Bitmap
    -> Inode Bitmap -> 
    Inode Table -> 
    Data blocks
    */
    printf("this is ram0 %d \n", ram0_fd);
    lseek(ram0_fd, SUPERBLOCK_OFFSET, SEEK_SET);

    if(-1 == read(ram0_fd,buffer,sizeof(struct ext2_super_block)))
    {
        printf("heyy buddy this failed\n");
    }

    
    return ram0_fd;
}

void ReadGroupDesc(int fd_to_use ,group_t * buffer)
{
    printf("this is ram0 %d \n", fd_to_use);
    lseek(fd_to_use,4096, SEEK_SET);
    printf("this is the sizeof(struct ext2_super) %ld \n", sizeof(struct ext2_super_block));
    printf("this is the sizeof(struct group desc) %ld \n", sizeof(struct ext2_group_desc));
    if(-1 == read(fd_to_use,buffer,sizeof(struct ext2_group_desc)))
    {
        printf("heyy buddy this failed\n");
    } 
}

     /*int                           fd;         the floppy disk file descriptor 
     int                           inode_no;   the inode number to read  
     const struct ext2_group_desc *group;      the block group to which the inode belongs 
     struct ext2_inode            *inode;      where to put the inode */
void ReadInode(int fd, int inode_no, group_t * group, inode_t * inode)
{
	lseek(fd, BLOCK_OFFSET(group->bg_inode_table)+(inode_no-1)*sizeof(struct ext2_inode), SEEK_SET);
	read(fd, inode, sizeof(struct ext2_inode));
}

void ReadRootFolder(int fd,group_t * group_descriptor, inode_t * inode)
{
    lseek(fd,BLOCK_OFFSET(group_descriptor->bg_inode_table)+(2)*sizeof(struct ext2_inode), SEEK_SET);
    read(fd, &inode, sizeof(struct ext2_inode));
}

void ReadBlock(int fd,inode_t inode, dir_entry_t dir, size_t size, int block)
{
    lseek(fd, BLOCK_OFFSET(inode.i_block[block]), SEEK_SET);
    read(fd, &dir, size);
}


/* 
void ReadDataBlockBit(int fd_to_use ,struct ext2_group_desc * buffer)
{
    printf("this is ram0 %d \n", fd_to_use);
    lseek(fd_to_use,BASE_OFFSET + sizeof(struct ext2_super_block) + sizeof(), SEEK_CUR);
    printf("this is the sizeof(struct ext2_super) %ld \n", sizeof(struct ext2_group_desc));
    if(-1 == read(fd_to_use,buffer,BASE_OFFSET))
    {
        printf("heyy buddy this failed\n");
    } 
}

void ReadInodeBitMap(int fd_to_use ,struct ext2_group_desc * buffer)
{
    printf("this is ram0 %d \n", fd_to_use);
    lseek(fd_to_use,BASE_OFFSET + sizeof(struct ext2_super_block), SEEK_CUR);
    printf("this is the sizeof(struct ext2_super) %ld \n", sizeof(struct ext2_super_block));
    if(-1 == read(fd_to_use,buffer,BASE_OFFSET))
    {
        printf("heyy buddy this failed\n");
    } 
}

void ReadInodeTable(int fd_to_use ,struct ext2_group_desc * buffer)
{
    printf("this is ram0 %d \n", fd_to_use);
    lseek(fd_to_use,BASE_OFFSET + sizeof(struct ext2_super_block), SEEK_CUR);
    printf("this is the sizeof(struct ext2_super) %ld \n", sizeof(struct ext2_super_block));
    if(-1 == read(fd_to_use,buffer,BASE_OFFSET))
    {
        printf("heyy buddy this failed\n");
    } 
} */