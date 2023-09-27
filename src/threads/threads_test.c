/*********************************
 * Reviewer: Dirak				
 * Author: Yohai Azoulai			
 * File: threads_test.c				
 * ******************************/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/*******Defines******/

#define ONE_HUNDRET_K 100000

/*******Structs******/

int g_s[ONE_HUNDRET_K];

/*******Service Funcs******/

void *Threadanator(void * index);
void ArrayPrinter();


/*******Funcs Funcs******/


int main(int argc, char *argv[])
{
    
    size_t i = 0;
    pthread_t thread_id;
    pthread_attr_t attr;

    pthread_attr_init (&attr);
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);

    while (i < ONE_HUNDRET_K)
    {
        while(0 != pthread_create(&thread_id, &attr, &Threadanator, (void *)i))
        {
            printf("inner loop");
            pthread_create(&thread_id, &attr, &Threadanator, (void *)i);
            
        }
        ++i;
    }

    sleep(10);
    ArrayPrinter();
   
    (void)argc;
    (void)argv;
    
    return 0;
}



void *Threadanator(void * index)
{
    g_s[(long)index] = (long)index;
    pthread_detach(pthread_self());
    return NULL;
}

void ArrayPrinter()
{
    size_t i = 0;
    for (i = 0; i < ONE_HUNDRET_K; i++)
    {
        printf("%d ,",g_s[i]);
    }
    
}