/*********************************
 * Reviewer: Dirak				
 * Author: Yohai Azoulai			
 * File: threads_test.c				
 * ******************************/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*******Defines******/

#define ONE_HUNDRET_K 100000
#define THREADZ 8
#define NUMBER 1000000000
#define RANGE (NUMBER/THREADZ)


/*******Structs******/

int g_s[ONE_HUNDRET_K];

typedef struct data_range
{
    size_t number;
    size_t from;
    size_t to;
}data_range_t;

/*******Service Funcs******/

void *Threadanator(void * index);
void ArrayPrinter();
void *CalcDiv(void *data_structure);



/*******Funcs Funcs******/


int main(int argc, char *argv[])
{
    
    size_t i = 0;
    pthread_t thread_id[THREADZ];
    data_range_t *struct_arr[THREADZ] = {NULL};
    void *return_value_arr[THREADZ] = {NULL};
    size_t results = 0;

    for (i = 0 ; i < THREADZ ; i++)
    {
        struct_arr[i] = (data_range_t *)malloc(sizeof(struct_arr));

        struct_arr[i]->from = i * RANGE;
        struct_arr[i]->to = (i + 1)*RANGE;
        struct_arr[i]->number = NUMBER;

    }

    struct_arr[0]->from = 1;
    
    for (i = 0; i < THREADZ; i++)
    {
        pthread_create(thread_id + i, NULL, CalcDiv, *(struct_arr + i));
    }

    for (i = 0; i < THREADZ ; i++)
    {
        pthread_join(thread_id[i], &return_value_arr[i]);
    }
    
    for (i = 0; i < THREADZ ; i++)
    {
        results += (size_t)(return_value_arr[i]);
        printf("this is result %ld , in the - %ld loop\n", results, i);
    }




/*     ArrayPrinter();
 */   
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

void *CalcDiv(void *data_structure)
{
    size_t i = 0;
    data_range_t *use_data = NULL;
    size_t sum_of_divisors = 0;
    use_data = (data_range_t *)data_structure;

    sum_of_divisors = 0;
    for (i = use_data->from; i < use_data->to ; i++)
    {
        if (0 == use_data->number % i)
        {
            sum_of_divisors += i;
        }
    }
    return (void *)sum_of_divisors;    
}


void ArrayPrinter()
{
    size_t i = 0;
    for (i = 0; i < ONE_HUNDRET_K; i++)
    {
        printf("%d ,",g_s[i]);
    }
    
}


void ArrayPrinter2()
{
    size_t i = 0;
    for (i = 0; i < THREADZ; i++)
    {
        /* code */
    }
    
}