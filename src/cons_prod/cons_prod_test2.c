/*********************************
 * Reviewer: DeepThought				
 * Author: Yohai Azoulai			
 * File: cons_prod_test.c				
 * ******************************/

#define _XOPEN_SOURCE 500
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc & free*/
#include <pthread.h> /*thread stuff*/
#include <stdio.h> /*file read stuff*/
#include <unistd.h> /*sleep*/
#include <semaphore.h> /*semaphores*/

#include "dll.h"
#include "cons_prod.h"

/*******Defines******/

#define NUM_THREADS 6
long in_counter;
long out_counter;
dll_t *list;
int g_value;
int prod_shutdown;

pthread_mutex_t thread_flag_mutex;


/*******Structs******/



/*******Service Funcs******/

void initialize_mutex_flag(void);

void *Producer(void *arg);

void *Consumer(void *arg);


/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int poison_apple = -1;
    size_t i = 0;
    g_value = 1;
    
    initialize_mutex_flag();
    list = DListCreate();

    
    for (i = 0; i < NUM_THREADS ; ++i)
    {
        if (0 == i || 1 == i)
        {
            if(0 != pthread_create(&threads[i], NULL, Consumer, NULL))
            {
                puts("ERROR: Failed to create the Consumer thread");
                exit(0);
            }
        }
        else
        {
            if (0 != pthread_create(&threads[i], NULL , Producer, NULL))
            {
                puts("ERROR: Failed to create the Producer thread");
                exit(0);
            }
        }
    }

    sleep(5);
    prod_shutdown = 1;

    for (i = 2 ; i < NUM_THREADS ; ++i)
    {
        if ((0 != pthread_join(threads[i], NULL)))
        {
            perror("Failed to join thread on Producers");
            exit(0);
        }
        
    }
    
    for (i = 0; i < 2 ; i++)
    {
        pthread_mutex_lock(&thread_flag_mutex);
        DListPushBack(list,&poison_apple);
        pthread_mutex_unlock(&thread_flag_mutex);
    }
    
    for (i = 0; i < 2; i++)
    {
        if ((0 != pthread_join(threads[i], NULL)))
        {
            perror("Failed to join thread on Consumers");
            exit(0);
        }
    }

    printf("in_counter + out_counter (should be 0) = %ld\n", (in_counter + out_counter));
    pthread_mutex_destroy(&thread_flag_mutex);
    DListDestroy(list);


    (void)argv;
    (void)argc;
    return 0;    
}

void *Producer(void *arg)
{
    int i = rand()%50;
    int p_flag = 1;

    while (p_flag)
    {
        usleep(rand()% 1000);
        printf("a message is ready\n");
        pthread_mutex_lock(&thread_flag_mutex);
        
        if(!DListPushBack(list,&i))
        {
            perror("Failed to PushBack");
            return NULL;
        }
        ++in_counter;
        printf("P : the value to be pushed %d\n",i);
        
        if (1 == prod_shutdown)
        {
            p_flag = 0;
        }
        pthread_mutex_unlock(&thread_flag_mutex);
    }
    
    (void)arg;
    return NULL;
}

void *Consumer(void *arg)
{
    
    while (1)
    {
        while (DListIsEmpty(list));
        pthread_mutex_lock(&thread_flag_mutex);
        if(!DListIsEmpty(list))
        {
            if (-1 == *(int *)DListGetData(DListBegin(list)))
            {
                pthread_mutex_unlock(&thread_flag_mutex);
                return NULL;
            }
            --out_counter;
            DListPopFront(list);
            printf("C : i've consumed a message\n");
            pthread_mutex_unlock(&thread_flag_mutex);
        }
        else
        {
            pthread_mutex_unlock(&thread_flag_mutex);
        }
    }
    (void)arg;
    return NULL;
}

void initialize_mutex_flag(void)
{
    pthread_mutex_init(&thread_flag_mutex,NULL);
    srand(time(NULL));
}
