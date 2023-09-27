/*********************************
 * Reviewer: Ya Rabi				
 * Author: Yohai Azoulai			
 * File: wdlib.c					
 * ******************************/
 

#define _POSIX_C_SOURCE 199309L
#define _DEFAULT_SOURCE
/*******Defines******/

#define NOT_USED(A) ((void)A)
#define CASTOT(A,B) ((A)B)


#define SEND_INTER 1

#define TIME_OUT 3

#define CHECK_SIG 5

#include <stdlib.h>     /*malloc & free*/
#include <fcntl.h>      /* For O_* constants */
#include <sys/stat.h>   /* For mode constants */
#include <sys/wait.h>   /* For mode constants */
#include <semaphore.h>
#include <signal.h>     /*signal*/
#include <pthread.h>    /*Threads*/
#include <stdatomic.h>  /*atomic increment*/
#include <stdio.h>      /*puts*/
#include <string.h>     /*memcpy*/


#include "wd.h"

#ifndef NDEBUG
char *wd_exec_name = "wd.Debug.out";
#else
char *wd_exec_name = "wd.Release.out";
#endif




atomic_int g_atomic_counter = 0;

atomic_int g_shutdown_flag = 0;



pid_t g_pid = 0;



/*******Structs******/

struct sigaction sig_action_struct = {0};

typedef struct
{
    scheduler_t *sched;
    sem_t *sync_maphore;
    pthread_t wd_thread;
    char **app_arguments;
}data_struct_t;
 
/*******Service Funcs******/

static void CallingSignalHandler(int sig);

static void DNRSignalHandler(int sig);

static int SendSignalTask(void *arg);

static int CheckIfGotSignalTask(void *arg);

static void *Initilize(int argc, void *argv[] , void *thread_data_struct);

static void *MMISchedulerTaskRun(void *arg);

static int IsThisTheWatchDog();

static int IsThisFirstInit();

static int Ressurection(void);

static void CleanUpFunc(void *arg);


/*******Funcs Funcs******/

void *MakeMeImmortal(int argc, char **argv)
{
    data_struct_t *transfer_data = NULL;

    if(NULL == (transfer_data = (data_struct_t *)Initilize(argc ,
                                            CASTOT(void *,&argv), transfer_data)))
    {
        printf("\033[0:32mfailed to initialize data stuff buddyyyyy\033[m\n");
        return NULL;
    }

    /**
     * this first if is meant to check who is calling MMI
     * if its the first time IsThisFirstInit() returns TRUE
     * g_pid recives the child process(wd) data in 247
     * g_pid for child proccess is 0 need ppid inside the ran func
    */
    
    if(IsThisFirstInit())
    {
        g_pid = fork();
        
        if(0 > g_pid)
        {
            printf("Error: in forking , this failed!!!!");
            return NULL;
        }
        
        if(0 == g_pid)
        {
            printf("child after fork current proccess in MMI, %d.\n\n",__LINE__);
            execlp(wd_exec_name, wd_exec_name,(char *)NULL);
        }
    }
    else
    {
        printf("g_pid Before Assignment - %d\n",g_pid);
        g_pid = getppid();
        printf("\033[1;37mAfter g_pid Assignment with ppid - %d\033[0m\n\n",g_pid);
    }
    
    printf("(getppid)Parent PID: %d\n",getpid());
    printf("(g_pid)child PID: %d\n",g_pid); 
    
    printf("MMI starting the RunTask! for  - %d , %d.\n\n",getpid(),__LINE__);
    pthread_create(&(transfer_data->wd_thread), NULL, MMISchedulerTaskRun, transfer_data);

    
    return transfer_data;
}

void DoNotRessurect(void *arg)
{
    int i = 0;
    struct timespec ts = {0};
    time_t interval_test = {0};

    if(!IsThisTheWatchDog())
    {
        g_shutdown_flag = 1;
        for (i = 0; i < TIME_OUT; ++i)
        {
            kill(g_pid ,SIGUSR2);
            
            printf("\033[1;101mInside the loop - %d\033[0m\n",i);
            
            ts.tv_sec = time(NULL) + 3;
            interval_test = time(NULL) + 3;
            while (interval_test >= time(NULL));
            
            printf("Prior to timewait - %ld.\n",interval_test);
            
            sem_timedwait(CASTOT(data_struct_t *,arg)->sync_maphore,&ts);
            printf("After - %ld.\n",interval_test);
        }
    }
        
    printf("\033[0;36mJoin inside DNR is next line!\033[0m\n");
    if(0 != pthread_join(CASTOT(data_struct_t *,arg)->wd_thread,NULL))
    {
        puts("Failed to join the WD thread");
    }
    
    CleanUpFunc(arg);

}

/*These is Initializing Func*/

static void *Initilize(int argc, void *argv[] , void *thread_data_struct)
{
    /*create Signal Handler*/
/*     int i = 0;
 */    int init_cleanup_flag = 0;
    ilrd_uid_t uid = {0};
    NOT_USED(argc);
    NOT_USED(argv);

    printf("\033[1:94mStarted Init Process.\033[0m\n\n");
    
    do
    {
        if(NULL == (thread_data_struct = malloc(sizeof(data_struct_t))))
        {
            printf("Malloc for arg struc failed\n");
            init_cleanup_flag = 1;
            break;
        }

        /* if(NULL == (CASTOT(data_struct_t *,thread_data_struct)->app_arguments = 
                                            malloc(sizeof(char *) * argc + 2)))
        {
            printf("Malloc for arg struc failed\n");
            init_cleanup_flag = 1;
            break;
        }

        for (i = 0; i < argc ; ++i)
        {
            CASTOT(data_struct_t *,thread_data_struct)->app_arguments = 
                                                            argv[i];
        }

        CASTOT(data_struct_t *,thread_data_struct)->app_arguments[argc + 1] = "\0"; */


        sig_action_struct.sa_flags = SA_RESTART;
        sig_action_struct.sa_handler = CallingSignalHandler;
        
        if(-1 == sigaction(SIGUSR1,&sig_action_struct, NULL))
        {
            printf("failed on SignalUSR1 Handler");
            init_cleanup_flag = 1;
            break;
        }
        
        sig_action_struct.sa_flags = SA_RESTART;
        sig_action_struct.sa_handler = DNRSignalHandler;
        
        if(-1 == sigaction(SIGUSR2,&sig_action_struct,NULL))
        {
            printf("failed on SignalUSR1 Handler");
            init_cleanup_flag = 1;
            break;
        } 
        
        if(SEM_FAILED == (CASTOT(data_struct_t *,thread_data_struct)->sync_maphore = 
                                    sem_open("/sync_maphore",O_CREAT,0666,0)))
        {
            printf("Semaphore creation failed\n");
            init_cleanup_flag = 1;
            break;
        }
        /*Create Schedular*/
        if(NULL == (CASTOT(data_struct_t *,thread_data_struct)->sched = 
                                                        SchedulerCreate()))
        {
            printf("failed on SchedulerCreate");
            init_cleanup_flag = 1;
            break;
        }

        uid=SchedulerAddTask(CASTOT(data_struct_t *,thread_data_struct)->sched, 
                                    SendSignalTask, 
                                    CASTOT(data_struct_t *,thread_data_struct), 
                                    SEND_INTER);
        if(UIDIsSame(g_bad_uid,uid))
        {
            printf("Scheduler Add Task failed");
            init_cleanup_flag = 1;
            break;
        }
        
        uid=SchedulerAddTask(CASTOT(data_struct_t *,thread_data_struct)->sched, 
                                    CheckIfGotSignalTask, 
                                    CASTOT(data_struct_t *,thread_data_struct), 
                                    CHECK_SIG);
        if(UIDIsSame(g_bad_uid,uid))
        {
            printf("Scheduler Add Task failed");
            init_cleanup_flag = 1;
            break;
        }
        printf("\033[1;96mFinished Init Process.\033[0m\n\n");


    }
    while(0);

    if(init_cleanup_flag)
    {
        CleanUpFunc(thread_data_struct);
    }
    return thread_data_struct;

}

/*These is the Signal Handlers*/

void CallingSignalHandler(int sig)
{
    NOT_USED(sig);
    g_atomic_counter = 0;
}

void DNRSignalHandler(int sig)
{
    NOT_USED(sig);
    g_shutdown_flag = 1;
}

/*These are the Scheduler Tasks! functions*/

int SendSignalTask(void *arg)
{
    /*wait for wd scheduler creation
    signal the other PID
    all this might need to be in a task?
    i really dont know*/
    atomic_fetch_add(&g_atomic_counter,1);
    printf("atomic_counter - %d\n",g_atomic_counter);
    printf("g_pid - %d\n",g_pid);
    
    kill(g_pid, SIGUSR1);

    NOT_USED(arg);
    return 0;
}

int CheckIfGotSignalTask(void *arg)
{
    printf("\033[1;92mCheck Signal Task.\033[0m\n");
    if (g_shutdown_flag)
    {
        printf("\033[0;103mDNR Requested!! Yassss!\033[0m\n");
        SchedulerStop(CASTOT(data_struct_t *,arg)->sched);
    }
    
    if(g_atomic_counter > 5)
    {   
        printf("\033[0;100mSomeone Died!\033[0m\n");
        SchedulerStop(CASTOT(data_struct_t *,arg)->sched);
    }
    return 0;
}

void *MMISchedulerTaskRun(void *arg)
{
    if(IsThisFirstInit())
    {
        printf("\033[0;100msem wait.\033[0m\n");
        
        sem_wait(CASTOT(data_struct_t *,arg)->sync_maphore);
    }
    else
    {
        printf("\033[0;101msem post.\033[0m\n");
        
        sem_post(CASTOT(data_struct_t *,arg)->sync_maphore);
    }
    while (1)
    {
        printf("\033[1;94mshecd RUN.\033[0m\n");
        
        SchedulerRun(CASTOT(data_struct_t *,arg)->sched);
        
        printf("\033[1;96mshecd Stopped.\033[0m\n");

        if (g_shutdown_flag)
        {
            break;
        }
        
        if (Ressurection())
        {
            printf("ressurection failed on fork\n");
            break;
        }
        sem_wait(CASTOT(data_struct_t *,arg)->sync_maphore);
    }

    return NULL;
}

/*These are the service functions*/

int IsThisFirstInit()
{
    return (getenv(ENV_NAME) == NULL);
}

int IsThisTheWatchDog()
{
    return (!IsThisFirstInit() && getpid() == atoi(getenv(ENV_NAME)));
}

int Ressurection(void)
{
   /*  if(g_shutdown_flag)
    {
        return 0;
    } */
    
    if (IsThisTheWatchDog())
    {
        /* You are the Dog that is watching */
        printf("\033[1;32mWatchDog Revive is GREEN.\033[0m\n");
        g_pid = fork();
        if (-1 == g_pid)
        {
            printf("fork failed on line %d.\n",__LINE__);
            return 1;
        }

        if(0 == g_pid)
        {
            printf("\033[1;32mSignal Check Revivel WatchDog (reviving the App) - %d.\033[0m\n",__LINE__);
            execlp("247.Debug.out", "247.Debug.out");
        }
    }
    else
    {
        g_pid = fork();
        printf("\033[1;31mApp Revive is Red.\033[0m\n");
        if (-1 == g_pid)
        {
            printf("\033[1;31mfork failed on line %d.\033[0m\n",__LINE__);
            return 1;
        }

        if(0 == g_pid)
        {
            printf("\033[1;31mSignal Check Revivel App (reviving the WD) - %d.\033[0m\n",__LINE__);
            execlp("wd.Debug.out", "wd.Debug.out");
        }
    }
    return 0;
}

void CleanUpFunc(void *arg)
{
    /**
    * Destroy Scheduler
    * Destroy Sem
    * IF Malloc Frees here
    */

    if(CASTOT(data_struct_t *,arg)->sync_maphore)
    {
        sem_close(CASTOT(data_struct_t *,arg)->sync_maphore);
        printf("\033[0;107;0;90msem_close removed\033[0m\n");
        sem_unlink("/sync_maphore");
        printf("\033[0;107;0;90msem_unlink\033[0m\n");
    }

    if(CASTOT(data_struct_t *,arg)->sched)
    {
        SchedulerDestroy(CASTOT(data_struct_t *,arg)->sched);
        printf("\033[0;107;0;90mSchedular Destruction\033[0m\n");
    }
    
    if(!IsThisFirstInit())
    {
        unsetenv(ENV_NAME);
        printf("\033[0;107;0;90mEnv Destruction Destruction\033[0m\n");
    }

    free(CASTOT(data_struct_t *,arg)->app_arguments);

    free(CASTOT(data_struct_t *,arg));

    arg = NULL;
    
    /* maybe use clearenv? if it doesnt 
    destroy the rest of the system*/
}