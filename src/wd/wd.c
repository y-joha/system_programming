/*********************************
 * Reviewer: Ishmor				
 * Author: Yohai Azoulai			
 * File: wd.c					
 * ******************************/
 
 /**/
#define _POSIX_C_SOURCE 199309L
#define _DEFAULT_SOURCE

#include <stdlib.h>

#include "wd.h"

/*******Defines******/
/**********&*********/
/*******Globals******/

#define SIZE 100

/*******Structs******/


/*******Service Funcs******/

void tostring(char str[], int num);

/*******Sync Params******/



/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    
    pid_t pid = getpid();
	char buffer[SIZE] = {'0'};
    void *data = NULL;

	tostring(buffer, pid);

    if(-1 == setenv(ENV_NAME, buffer, 1))
    {
        printf("WD_PID environment variable creation failed");
    } 
    printf("\033[0;103m WatchDog MMI call\033[0m\n");

    data = MakeMeImmortal(argc , argv);
    
    DoNotRessurect(data);
    
    return 0;
}

void tostring(char str[], int num)
{
    int i = 0, rem = 0, len = 0, n = 0;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

