/*********************************
 * Reviewer: Ishmor				
 * Author: Yohai Azoulai			
 * File: wd.c					
 * ******************************/

#ifndef __WD_H__
#define __WD_H__

#include <stdlib.h>     /*malloc & free*/
#include <unistd.h>     /*pid_t*/
#include <fcntl.h>      /* For O_* constants */
#include <sys/stat.h>   /* For mode constants */
#include <semaphore.h>  /*semaphores*/
#include <signal.h>     /*signal*/
#include <pthread.h>    /*Threads*/
#include <stdio.h>      /*puts*/

#define ENV_NAME "WD_PID"


extern int MakeMeImmortal(int argc, char *argv[]);



extern void DoNotRessurect(pid_t watch_dog_pid);

#endif 


