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
#define SIZE 20
#define NUM_THREADS 3

long in_counter;
long out_counter;



sem_t semCond;

pthread_mutex_t condMutex;

pthread_cond_t condTry;

int message;

int message_flag;


/*******Structs******/



/*******Service Funcs******/

void initialize_flags(void);

void *Producer(void *arg);

void *Consumer(void *arg);

void WriteToBuffer(int var);

int ReadToBuffer(void);



/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int poison_apple = -1;
    size_t i = 0;
    
    initialize_flags();

    
    for (i = 0; i < NUM_THREADS ; ++i)
    {
        if (0 == i)
        {
            if(0 != pthread_create(&threads[i], NULL, Producer, NULL))
            {
                puts("ERROR: Failed to create the Consumer thread");
                exit(0);
            }
        }
        else
        {
            if (0 != pthread_create(&threads[i], NULL , Consumer, (void *)i))
            {
                puts("ERROR: Failed to create the Producer thread");
                exit(0);
            }
        }
    }

    sleep(3);
    printf("before taking lock\n");
    pthread_mutex_lock(&condMutex);
    printf("after taking lock\n");
    message = poison_apple;
    printf("before release lock\n");
    pthread_mutex_unlock(&condMutex);
    pthread_cond_broadcast(&condTry);
    printf("after release lock\n");
    

    for (i = 1 ; i < NUM_THREADS ; ++i)
    {
        if ((0 != pthread_join(threads[i], NULL)))
        {
            perror("Failed to join thread on Consumers");
            exit(0);
        }
    }
    
    printf("done with prod joining\n");
    printf("\n");
    

    if ((0 != pthread_join(threads[0], NULL)))
    {
        perror("Failed to join thread on Producers");
        exit(0);
    }

    printf("done with poison pushing\n");
    printf("in_counter + out_counter (should be 0) = %ld\n", (in_counter + out_counter));
    pthread_mutex_destroy(&condMutex);
    sem_destroy(&semCond);


    (void)argv;
    (void)argc;
    return 0;    
}

void *Producer(void *arg)
{
    size_t i = 0;

    while (1)
    {
        int j = rand()%50;
        message_flag = 0;
        usleep(rand()% 1000);
        for (i = 0; i < NUM_THREADS - 1; ++i)
        {
            sem_wait(&semCond);
        }
        
        pthread_mutex_lock(&condMutex);
        
        if (-1 == message)
        {
            pthread_mutex_unlock(&condMutex);
            break;
        }

        WriteToBuffer(j);
        printf("********** P ********\n");
        printf("P : a message is ready to be read\n");
        message_flag = 1;

        in_counter += NUM_THREADS - 1;


        pthread_mutex_unlock(&condMutex);

        pthread_cond_broadcast(&condTry);
    }

    (void)arg;
    return NULL;
}

void *Consumer(void *arg)
{
    
    while (1)
    {
        pthread_mutex_lock(&condMutex);
        
        sem_post(&semCond);
        
        pthread_cond_wait(&condTry,&condMutex);
        
        if (-1 == message)
        {
            pthread_mutex_unlock(&condMutex);
            break;
        }
        
        /* ReadToBuffer(); */
        printf("\n");
        printf("C %ld\n the message to be read is %d\n",(long)arg ,message);
        printf("C : i've consumed a message\n");
        --out_counter;
        pthread_mutex_unlock(&condMutex);
    }
    (void)arg;
    return NULL;
}

void initialize_flags(void)
{
    pthread_mutex_init(&condMutex,NULL);
    sem_init(&semCond,0,0);
    pthread_cond_init(&condTry,NULL);
    srand(time(NULL));
}


void WriteToBuffer(int var)
{
    message = var;
}

/* int ReadToBuffer(void)
{
    int tmp = 0;
    tmp = array[startread];
    startread += 1;
    startread %= SIZE; 
    return tmp;
} */