/*********************************
 * Reviewer: Kobi Oz				
 * Author: Yohai Azoulai			
 * File: s&s.c					
 * ******************************/
 
 /**/

#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/
#include <pthread.h>   /*thread stuff*/
#include <unistd.h>    /*mmap macros */
#include <semaphore.h> /*semaphores*/
#include <stdio.h>     /* fopen */
#include <fcntl.h>     /* open flags */
#include <sys/mman.h>  /* mmap */
#include <sys/stat.h>  /* mmap */



/*******Defines******/

#define CASTOT(A,B) ((A)B)
#define NOT_USED(A) ((void)A)

#define PAGE_SIZE 4096

/*******Structs******/



/*******Service Funcs******/



/*******Funcs Funcs******/

int main(int argc, char **argv)
{
    int file_dict_refrence = NULL;
    char *dictionary = NULL;
    struct stat dic_inod = {0};

    file_dict_refrence = open("/usr/share/dict/words", O_RDONLY, S_IRUSR | S_IWUSR);

    if(-1 == fstat(file_dict_refrence, &dic_inod))
    {
        perror("fstat");
    }
    printf("the file size is  %ld\n",dic_inod.st_size);

    dictionary = mmap(NULL,dic_inod.st_size,PROT_READ, MAP_SHARED, file_dict_refrence, 0);

    printf("data within dictionary is\n");
    printf("%s");
    printf("\n");

    NOT_USED(argc);
    NOT_USED(argv);
}