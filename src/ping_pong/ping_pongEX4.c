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
static int ping_pong_ball = 0;


/*******Structs******/

struct sigaction sa = {0};

pid_t pid;

/*******Service Funcs******/

void ping_pong_Ex4(int signal, siginfo_t *info, void *ucontext)
{
    (void)ucontext;
    (void)signal;
    pid = info->si_signo;
    ping_pong_ball = 1;
}

/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    sa.sa_sigaction = &ping_pong_Ex4;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2, &sa, NULL);
    /* parent part */
    
    printf("pingEX4.out pid:%d\n", getpid());

    while (1)
    {
        printf("ping\n");
        usleep(200);
        ping_pong_ball = 0;
        
        while(!ping_pong_ball);
        kill(pid, SIGUSR1);
        printf("a pid : %d\n", pid);
    }
    
    (void)argc;
    (void)argv;
    
    return 0;
}