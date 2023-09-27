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
#define NUMBER 1000000000


/*******Structs******/


/*******Service Funcs******/

size_t CalcDiv();



/*******Funcs Funcs******/


int main(int argc, char *argv[])
{
    
    printf("the result is %ld\n", CalcDiv());

    (void)argc;
    (void)argv;
    
    return 0;
}



size_t CalcDiv()
{
    size_t i = 0;
    size_t sum_of_divisors = 0;

    for (i = 1; i < NUMBER ; ++i)
    {
        if (0 == NUMBER % i)
        {
            sum_of_divisors += i;
        }
    }
    return sum_of_divisors;    
}
