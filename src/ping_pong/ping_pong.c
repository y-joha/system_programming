/*********************************
 * Reviewer: Ding Dong				
 * Author: Yohai Azoulai			
 * File: ping_pong.c					
 * ******************************/
 
 /**/
#define _POSIX_C_SOURCE 199309L

#include <stdlib.h> /*malloc & free*/
#include <unistd.h>
#include <semaphore.h> /*semaphores*/
#include <signal.h> /*signal*/
#include <stdio.h> /*printf*/


/*******Defines******/
/*if its 0 its the parent*/
static int ping_pong_ball = 1;


/*******Structs******/

struct sigaction sa = {0};


/*******Service Funcs******/

void ping_pong_handler(int signal)
{
    if (SIGUSR1 == signal)
    {
        ping_pong_ball = 0;
    }
    else
    {
        ping_pong_ball = 1;
    }
}

/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    pid_t pid;

    sa.sa_handler = &ping_pong_handler;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    if(-1 == (pid = fork()))
    {
        return -1;
    }

    if(0 == pid)
    {
        /*child part*/
        while (1)
        {
            while(1 == ping_pong_ball);
            printf("pong\n");
            usleep(20);
            kill(getppid(), SIGUSR2);
            pause();
        }
    }
    else
    {
        /* parent part */
        while (1)
        {
            while(0 == ping_pong_ball);
            printf("ping\n");
            usleep(20);
            kill(pid, SIGUSR1);
            pause();
        }
    }

    
    (void)argc;
    (void)argv;
    
    
    return 0;
}