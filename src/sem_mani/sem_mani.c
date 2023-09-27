/*********************************
 * Reviewer: Linus Tourvald				
 * Author: Yohai Azoulai			
 * File: semmanip.c					
 * ******************************/
 
 /**/

#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc & free*/
#include <pthread.h> /*thread stuff*/
#include <ctype.h> /*toupper*/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h> /*semaphores*/
#include <fcntl.h> /*Sem Flags values*/

#include "sem_mani.h"
/*******Defines******/



/*******Structs******/



/*******Service Funcs******/

static int GetValue(char *string);


/*******Funcs Funcs******/

int SemaMan(char *name)
{
    
    sem_t *sem_name = 0;
    char buffer[50];
    int sem_value = 0;
    int interval = 0;
    
    
    sem_name = sem_open(name, O_CREAT, 0666 , 0);
    
    
    if(sem_name == SEM_FAILED)
    {
        puts("failed inside the func\n");
        exit(1);
    }

    while (1)
    {
        fgets(buffer, 5,stdin);
        interval = GetValue(buffer);
        
        switch (toupper(*buffer))
        {
            case 'D':
                while (interval)
                {
                    sem_trywait(sem_name);
                    --interval;
                }
                break;

            case 'I':
                while (interval)
                {
                    sem_post(sem_name);
                    --interval;
                }
                break;

            case 'V':
                printf("the value of the semaphore %s \n", name);
                sem_getvalue(sem_name,&sem_value);
                printf("is the value - %d\n", sem_value);
                break;    
            
            case 'X':
                sem_close(sem_name);
                sem_unlink(name);
                exit(0);
                break;
            
            default:
                break;
        }
    
    }
    return 0;    
}

int GetValue(char *buffer)
{
    return atoi(buffer + 1);
}
    
