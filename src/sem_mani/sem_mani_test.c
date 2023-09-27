/*********************************
 * Reviewer: Linus Tourvald				
 * Author: Yohai Azoulai			
 * File: semmanip_test.c				
 * ******************************/

#include "sem_mani.h" 
#include <stdio.h>

/*******Defines******/



/*******Structs******/



/*******Service Funcs******/



/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        puts("failed in main\n");
        
        return 1;
    }

    SemaMan(argv[1]);
    

    return 0;
}
