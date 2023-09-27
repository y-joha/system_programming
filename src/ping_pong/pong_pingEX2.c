/*********************************
 * Reviewer: Ding Dong				
 * Author: Yohai Azoulai			
 * File: ping_pong.c					
 * ******************************/
 
 /**/
#define _POSIX_C_SOURCE 199309L
#define _DEFAULT_SOURCE

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
    (void)signal;
    ping_pong_ball = !ping_pong_ball;
}

/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    sa.sa_handler = &ping_pong_handler;
    sigaction(SIGUSR1, &sa, NULL);
    
    while (1)
    {
        ping_pong_ball = 1;
        while(ping_pong_ball);
        printf("pong\n");
        usleep(200);        
        kill(getppid(), SIGUSR2);
    }
    
    (void)argc;
    (void)argv;
    
    
    return 0;
}