/*********************************
 * Reviewer: Kobi				
 * Author: Yohai Azoulai			
 * File: vsa_new.c					
 * ******************************/
 
 /**/

#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc & free*/
#include <stdio.h>

#include "vsa.h"

/*******Define's******/

#define WORD_V (sizeof(size_t))

/*******Structs******/
struct vsa
{
    size_t pool_size;
};
struct block_meta
{
    size_t block_size;

    #ifndef NDEBUG
    void *magic_number;
    #endif
};

typedef struct block_meta block_meta_t;

/*******Service Funcs******/

static void *Align(size_t what, size_t to);

static void *VSADeFragmentation(vsa_t *vsa, size_t requested_size);


/*******Funcs Funcs******/

vsa_t *VSAInit(void *pool, size_t pool_size)
{
    vsa_t *vsa = NULL;
    block_meta_t *block = NULL;

    size_t address_diff = 0;
    size_t size_for_user = 0;

    assert(pool);
    /* line 48 rounds up to 8   63 -> 64*/
    vsa = (vsa_t *)Align((size_t)pool,WORD_V);
    address_diff = (size_t)vsa - (size_t)pool;
    
    /* test step - size recived at least >= 0*/

    if(pool_size  < (size_t)Align(sizeof(vsa_t),WORD_V) + (size_t)Align(sizeof(block_meta_t),WORD_V) + address_diff)
    {
        return NULL;
    }
    
    size_for_user = pool_size - address_diff - (size_t)Align(sizeof(vsa_t),WORD_V);
    /* oppose to line 48 rounds down!!!! to 10 -> 8*/
    size_for_user &= ~(WORD_V -1);
    /* initialazation step */

    vsa->pool_size = size_for_user;
    block = (block_meta_t *)((char *)vsa + (size_t)Align(sizeof(vsa_t),WORD_V));


    /* cast to <=|void *|=> so it will be read as
        pointer  */
    #ifndef NDEBUG
    block->magic_number = (void *)555;
    #endif
    
    block->block_size = size_for_user;

    return vsa;
}

void *VSAAlloc(vsa_t *vsa, size_t variable_size)
{
    block_meta_t *block_a = NULL;
    block_meta_t *block_b = NULL;
    

    size_t align_size_requested = 0;
    
    assert(vsa);
    
    align_size_requested = (size_t)Align(variable_size,WORD_V);
    block_a = (block_meta_t *)VSADeFragmentation(vsa, align_size_requested);
    

    if(!block_a)
    {
        return NULL;
    }


    block_b = (block_meta_t *)((char *)block_a + align_size_requested + (size_t)Align(sizeof(block_meta_t),WORD_V));

    if(block_a->block_size - align_size_requested > (size_t)Align(sizeof(block_meta_t),WORD_V) + WORD_V)
    {
        block_b->block_size = block_a->block_size - align_size_requested - (size_t)Align(sizeof(block_meta_t),WORD_V);
        #ifndef NDEBUG
        block_b->magic_number = (void *)555;
        #endif
    }

    block_a->block_size = align_size_requested ^((size_t)1); /* i think u better cast the literal number into size_t || Corrected*/

    return (char *)block_a + (size_t)Align(sizeof(block_meta_t),WORD_V);

}

void VSAFree(void *block)
{
    assert(block);
    block = (block_meta_t *)((char *)block - (size_t)Align(sizeof(block_meta_t),WORD_V));

    /* u already asserted block at line 121 || Corrected*/
    assert(((block_meta_t *)block)->magic_number == (void *)555);

    ((block_meta_t *)block)->block_size &= (size_t)(~1);


}

size_t VSALargestChunckAvailable(vsa_t *vsa)
{
    block_meta_t *block_a = NULL;
    size_t biggest_found = 0;
    size_t mask = 1;

    assert(vsa);
    VSADeFragmentation(vsa,vsa->pool_size);

    block_a = (block_meta_t *)((char *)vsa + (size_t)Align(sizeof(vsa_t),WORD_V));

    while((char *)vsa + (size_t)Align(sizeof(vsa), WORD_V) > (char *)block_a)
    {
        if(!(block_a->block_size & mask) && block_a->block_size > biggest_found)
        {
            biggest_found = block_a->block_size;
        }
        block_a = (block_meta_t *)((char *)block_a + (size_t)Align(sizeof(block_meta_t),WORD_V) + block_a->block_size);
    }
    return biggest_found;

}

    /**
     * check first block -> free 
     * (1)are you free and too big?  (!(block & 1) && (block_a->block_size - align_size_requested > (size_t)Align(sizeof(block_meta_t),WORD_V) + WORD_V))
     * (2)    yes -> split the block and return pointer
     * (3)     no -> if you are free but not big enough is following free?
     * (4)             yes -> combine them, are you enough now?
     * (5)               yes -> do you have enough for more than ((size_t)Align(sizeof(block_meta_t),WORD_V) + WORD_V + requested_size)
     * (6)               no -> is the next one in line free?
     * (7)                  yes -> repeate
     * (8)                  no  -> continue till you find next free
     * (9)             no  -> continue till you find next free
     * */
static void *VSADeFragmentation(vsa_t *vsa, size_t requested_size)
{

    block_meta_t *block_a = NULL;
    block_meta_t *block_b = NULL; 
    size_t mask = 1;

    assert(vsa);
    /*first block in VSA*/
    block_a = (block_meta_t *)((char *)vsa + (size_t)Align(sizeof(vsa_t),WORD_V));
    while(((char *)vsa + vsa->pool_size) > (char *)block_a)
    {
        /* (1) are you free & too big?*/
        if(!(block_a->block_size & mask) && (block_a->block_size > requested_size + (size_t)Align(sizeof(block_meta_t),WORD_V)))
        {
            return block_a;
        }
        /* vlg will warn here unconditioniod*/
        
        block_b = (block_meta_t *)((char *)block_a + (block_a->block_size &(~mask)) + (size_t)Align(sizeof(block_meta_t),WORD_V));
        /* check if block_b is valid*/
        if(((char *)vsa + vsa->pool_size) > (char *)block_b)
        {
            /* check if blocks are free and join*/
            if(!(block_a->block_size & mask) && (!(block_b->block_size & mask)))
            {
                block_a->block_size += block_b->block_size + (size_t)Align(sizeof(block_meta_t),WORD_V);
            }
            /* check if blocks are free and join*/
            else
            {
                block_a = block_b;
            }
        }
        else
        {
            return NULL;
        }
    }
    return NULL;
}

static void *Align(size_t what, size_t to)
{
    return (void *)((what - 1 + to) & - to);
}
