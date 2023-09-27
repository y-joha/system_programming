/*********************************
 * Reviewer: 				
 * Author: Yohai Azoulai			
 * File: fsa.c					
 * ******************************/
 
 /**/
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include "fsa.h"

#define WORD (sizeof(size_t))
#define FSA_ALINGED (size_t)Align(sizeof(fsa_t),WORD)

/*******Structs******/
struct fsa
{
    size_t offset;
};


/*******Service Funcs******/

static void* Align(size_t what, size_t to);

/*******Funcs Funcs******/

fsa_t *FSAInit(void *pool, size_t pool_size, size_t block_size)
{
    fsa_t *fsa = NULL;
    char *start_of_block = NULL;
    size_t i = 1;
    
    assert(pool);

    fsa = Align((size_t)pool,WORD);
    fsa->offset = 0;
    block_size = (size_t)Align(block_size, WORD);
    /*write the adress of fsa + sizeof(fsa_t)*/
    start_of_block = (((char *)fsa) + (size_t)Align(sizeof(fsa_t),
                                                            WORD));

    fsa->offset = 0;

/*    printf("%s\n", start_of_block);
*//*    printf("%ld before the loop\n",(size_t)*start_of_block);
*/  
    if(pool_size<(block_size  + ((size_t)start_of_block - (size_t)pool)))
    {
        return NULL;
    }


    pool_size = pool_size - ((size_t)start_of_block - (size_t)pool);

    while(pool_size >= 2*block_size)
    {
        *(size_t *)start_of_block = (block_size * i);
       ++i;
        start_of_block += block_size;
        
        pool_size -= block_size;
    }
    
    *(int *)(start_of_block) = -1 ;


    return fsa;


}

void *FSAAlloc(fsa_t *fsa)
{
    void *temp = NULL;
    assert(fsa);
    

    if(-1 == (int)fsa->offset)
    {
        return NULL;
    }

    temp = (char *)fsa + FSA_ALINGED+ fsa->offset;
    fsa->offset = *(size_t *)((char *)fsa + FSA_ALINGED + fsa->offset);
    return temp;
}

void FSAFree(fsa_t *fsa, void *block_address)
{
    assert(fsa);
    assert(block_address);
    *(size_t *)block_address = fsa->offset;
    fsa->offset = (size_t)block_address -
                         (size_t)(((char *)fsa) + FSA_ALINGED);
}

size_t FSACountFree(fsa_t *fsa)
{
    size_t counter = 0;
    size_t temp = 0;

    assert(fsa);

    temp = fsa->offset;
    while((-1 != (int)temp))
    {
        temp = *(size_t *)((char *)fsa + (FSA_ALINGED + temp));
        ++counter;
    }

    return counter;
}

/* Here we need to align block to size + man_struct + */
size_t FSASuggestSize(size_t block_size, size_t num_of_blocks)
{
    return ((size_t)Align(block_size,WORD)) * num_of_blocks + 
                                                        FSA_ALINGED
                                                        +WORD - 1; 
}

static void* Align(size_t what, size_t to)
{
    return (void*)((what - 1 + to) & -to);
}













/*static size_t EffectiveSize(void *pool, size_t pool_size, size_t block_size)
{
    size_t end_of_pool = 0;
    fsa_t *fsa = Align(pool,WORD);
    end_of_pool = (size_t *)pool + pool_size;
    return (end_of_pool - (pool + fsa + WORD))block_size;
}


fsa_t *FSAInit(void *pool, size_t pool_size, size_t block_size)
{
    fsa_t *fsa = NULL;
    size_t node_starting_offset = 0, size_t num_blocks = 0;
    char *block = NULL;

    assert(pool);

    fsa = (fsa_t *)Align(pool,WORD);
    node_starting_offset = fsa + sizeof(fsa_t);
    fsa->offset = (size_t)Align(fsa->offset + sizeof(fsa_t), (size_t));
    num_blocks = EffectiveSize(pool,pool_size,block_size);

    while(num_blocks)
    {

    }
    block_size = (size_t)Align(block_size, WORD);


    block = (char *)fsa + sizeof(fsa);

    pool_size -= (char *)block - (char *)pool;

    while(i < pool_size - 1)
    {
        man_struct.offset = 
    }

    *(int *)block = -1;
    return ;

    
}
*/