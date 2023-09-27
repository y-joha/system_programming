/*********************************
 * Reviewer: DeepThought				
 * Author: Yohai Azoulai			
 * File: cons_prod_test.c				
 * ******************************/

#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc & free*/
#include <pthread.h> /*thread stuff*/
#include <stdio.h> /*file read stuff*/
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h> /*semaphores*/
#include "cons_prod.h" 
/*******Defines******/

#define NUM_THREADS 2
int buffer_pass = 3;
volatile int g_value = 1;

/*******Structs******/



/*******Service Funcs******/

void *Producer(void *arg);

void *Consumer(void *arg);


/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    size_t i = 0;

    for (i = 0; i < NUM_THREADS ; ++i)
    {
        if (0 == i)
        {
            if(0 != pthread_create(&threads[i], NULL, Consumer, NULL))
            {
                puts("ERROR: Failed to create the Consumer thread");
                exit(0);
            }
            
        }
        
        if (0 != pthread_create(&threads[i], NULL , Producer, NULL))
        {
            puts("ERROR: Failed to create the Producer thread");
            exit(0);
        }
    }

    for (i = 0 ; i < NUM_THREADS ; ++i)
    {
        if (0 != pthread_join(threads[i], NULL))
        {
            perror("Failed to join thread");
            exit(0);
        }
    }
    (void)argc;
    (void)argv;
    return 0;    
}


/* void SingleProducerConsumer(char *name)
{
    pthread_t thread_id = 0;

    
} */

void *Producer(void *arg)
{
    int check_flag = 0;
    while (__atomic_load(&g_value,&check_flag,__ATOMIC_SEQ_CST),0 == g_value);
    printf("a message is ready\n");
    g_value = 0;
    return Producer(arg);
}

void *Consumer(void *arg)
{
    int check_flag = 0;
    while (__atomic_load(&g_value,&check_flag,__ATOMIC_SEQ_CST),1 == g_value);
    printf("i've consumed a message\n");
    g_value = 1;
    return Consumer(arg);
}