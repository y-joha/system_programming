/*********************************
 * Reviewer: GrandMiezer				
 * Author: Yohai Azoulai			
 * File: ext2_test.c				
 * ******************************/
#include "ext2.h"

/*******Defines******/

int file_descriptor;
group_t group_desc;
super_t supa_block;
inode_t inode_to_use;

/*******Structs******/



/*******Service Funcs******/
unsigned int block_size;


/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    group_t group_desc = {0};
    super_t supa_block ={0};
    inode_t inode_ctype_ptr = {0};
    inode_t * inode_root_ptr = NULL;
    dir_entry_t * dir_ptr = NULL;
    dir_entry_t * ctype_ptr = NULL;
    int fd_ram0 = 0;

    dir_ptr = (dir_entry_t *)malloc(8192);
    ctype_ptr = (dir_entry_t *)malloc(8192);
    inode_root_ptr = (inode_t *)malloc(8192);


    fd_ram0 = ReadMetatoBuffer(&supa_block);

    /* calculate number of block groups on the disk */
/* calculate size of the group descriptor list in bytes */
    ReadGroupDesc(fd_ram0,&group_desc);
    block_size = 1024 << supa_block.s_log_block_size;

    
/*     ReadInode(fd_ram0,5,&group_desc,&inode_root_ptr);
 */    
    
    ReadRootFolder(fd_ram0,&group_desc,inode_root_ptr);

   
   
    /* printf("Num of Blocks: %d\n", supa_block.s_blocks_per_group);
	printf("size of Blocks: %d\n", supa_block.s_log_block_size);
    printf("num of free blocks: %d\n", supa_block.s_free_blocks_count);
    printf("address of super_block pointer %p\n", supa_block);
    

    printf("address of group descriptor ptr %p\n", group_desc);

    printf("number of Block Groups is: %d\n", group_count);
    printf("Group discription list size in bytes: %d\n", descr_list_size);
 */
   
    printf("GD block bitmap : %d\n", group_desc.bg_block_bitmap);
    printf("GD inode bitmap : %d\n", group_desc.bg_inode_bitmap);
    printf("GD inode table : %d\n", group_desc.bg_inode_table);

    printf("i_blocks from i_node : %2u\n", inode_root_ptr->i_block[0]);
    if(S_ISDIR(inode_root_ptr->i_blocks))
    {
        printf("this is Dir\n");
    }


    ReadBlock(fd_ram0,*inode_root_ptr, *dir_ptr, block_size, 2);


    

    /* while(0 != dir_ptr->name_len) 
    {
        
        printf("%10u %.*s\n", dir_ptr->inode, dir_ptr->name_len, (dir_ptr)->name);
        dir_ptr = (dir_entry_t *)((char *)dir_ptr + dir_ptr->rec_len);
    } */
    
    lseek(fd_ram0,BLOCK_OFFSET(group_desc.bg_inode_table)+(46)*sizeof(struct ext2_inode), SEEK_SET);
    read(fd_ram0, &inode_ctype_ptr, sizeof(struct ext2_inode));

    lseek(fd_ram0, BLOCK_OFFSET(inode_ctype_ptr.i_block[0]), SEEK_SET);
    read(fd_ram0, ctype_ptr, block_size);

    printf("%10u %.*s\n", ctype_ptr->inode, ctype_ptr->name_len, (ctype_ptr)->name);



  /*   while (index < inode_ctype_ptr.i_size)
    {
        printf("%10u %.*s\n", ctype_ptr->inode, ctype_ptr->name_len, (ctype_ptr)->name);
        ctype_ptr = (dir_entry_t *)((char *)ctype_ptr + ctype_ptr->rec_len);
    }
     */


    (void)argc;
    (void)argv;

    return 0;
}
