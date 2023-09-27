/*********************************
 * Reviewer: Elohim				
 * Author: Yohai Azoulai			
 * File: 247.c					
 * ******************************/
 
 /**/


#include "wd.h"
/*******Defines******/


/*******Structs******/



/*******Service Funcs******/



/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    int i = 0;
    void *data = NULL;
    printf("\033[0;105m App MMI Call \033[0m\n");
    data = MakeMeImmortal(argc, argv);

    for (i = 0; i < 30; ++i)
    {
        sleep(10);
        printf("\033[1;95m App Sleep Cycle \033[0m\n");
    }
    

    DoNotRessurect(data);

    return 0;
}