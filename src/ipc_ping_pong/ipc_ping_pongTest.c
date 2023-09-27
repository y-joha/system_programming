/*********************************
 * Reviewer: I aint touching that with a 6ft. pole				
 * Author: Yohai Azoulai			
 * File: ipc_ping_pong_test.c				
 * ******************************/


#include <sys/types.h> /* fork - named-pipe*/
#include <sys/stat.h> /*named-pipe*/
#include <sys/wait.h> /* wait procc*/
#include <fcntl.h> /*Sem Flags values & open()*/
#include <unistd.h> /* fork */
#include <stdio.h> /* printf */
#include <string.h> /* strlen*/
#include <semaphore.h> /*semaphore*/
#include <mqueue.h>    /*message queue*/
#include <sys/mman.h>  /* mmap stuff */

/*******Defines******/


char *ping = "ping";
char *pong = "pong";
char *zaow = "zaow";
char *mq_name = "/messagequeue";

char *FIFO_set_a = "IsoFile";
char *FIFO_set_b = "SecFile";

mqd_t mqd_iso = 0;
struct mq_attr mq_struct = {0};
char g_mila[10];

int g_pid;
int g_fd_pipe_set_a[2];
int g_fd_pipe_set_b[2];
int g_fd_named_pipe[2];
char *name  = "sam";
sem_t * pong_for_the = 0;
char *name1  = "I Am";
sem_t * for_the_ping = 0;


char *phrase[] = 
{
    "god",
    "damn",
    "it",
    "cage"
};

struct Message
{
    char first[6];
    char sec[6];
    char third[4];
    char forth[5];
}habi_bale;

int shm_fd;
char *shm_name = "Honky_Tonk";
char *shm_ptr;
struct stat shm_inod = {0};

/*******Structs******/

/*Excersice - */
#if 0

#endif


/*******Service Funcs******/

int Init();
void Eradicate();
void ResetColor();
void RedColor();
void GreenColor();


/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    
    if(-1 == Init()){printf("go fund yourself\n"); return 1;}
    
    /*Excersice - 2*/
    #if 0
    if(0 > (g_pid = fork()))
    {
        return 1;
    }

    #endif


    /*Excersice - 2*/
    #if 0
    if (0 == g_pid)
    {
        /*Child proccess write ping 
            prints pong*/
        while (1)
        {
            write(g_fd_pipe_set_a[1],ping,strlen(ping));
            /* sem_post(for_the_ping);
            usleep(1);
            sem_trywait(pong_for_the); */
            RedColor();
            read(g_fd_pipe_set_b[0],g_mila,10);
            printf("%s - child\n",g_mila);
            ResetColor();
            

        }
    }
    #endif

    /*parent proccess write pong 
            prints ping*/

    while (0)
    {
        /*Excersice - 2*/
        #if 0
        /* sem_wait(for_the_ping); */
        GreenColor();
        read(g_fd_pipe_set_a[0],g_mila,10);
        printf("%s - parent\n",g_mila);
        write(g_fd_pipe_set_b[1],pong,strlen(pong));
        ResetColor();
        /* sem_post(pong_for_the); */
        #endif

        /*Excersice - 3*/
        #if 0
        write(g_fd_named_pipe[1],zaow,5);
        RedColor();
        read(g_fd_named_pipe[0],g_mila,5);
        printf("%s - iso - Should be zaga\n",g_mila);
        ResetColor();
        #endif


    }

    /*Excersice - 4*/
    #if 0
    for (size_t i = 0; i < 5; ++i)
    {
        printf("reached loop \n");
        if(-1 == mq_send(mqd_iso,phrase[i],strlen(phrase[i]),1)){perror("mq_send");}
        printf("message %ld : written to buffer \n",i + 1);
    }
    
    #endif 

    /*Excersice - 5*/
    #if 1
    
    /* Writing the Message to the shared memory */

    

    for (size_t i = 0; i < 4; ++i , shm_ptr += (strlen(phrase[i - 1]) + 1))
    {
        sprintf(shm_ptr ,"%s",phrase[i]);
    }
    munmap(shm_ptr,shm_inod.st_size);
    #endif


/*     Eradicate();
 */
    (void)argc;
    (void)argv;

    return 0;
}

int Init()
{
    /*Excersice - 2*/
    #if 0

    /* Init of the Pipe Fd*/
    if(-1 == pipe(g_fd_pipe_set_a))
    {
        printf("Failed to open pipe");
        return 1;
    }

    if(-1 == pipe(g_fd_pipe_set_b))
    {
        printf("Failed to open pipe");
        return 1;
    }
    #endif

    /*Excersice - 3*/
    #if 0
    /* create FIFO file*/
    if(-1 == (mkfifo(FIFO_set_a, 0666))){printf("Failed to create a FIFO File\n"); return 1;}
    if(-1 == (mkfifo(FIFO_set_b, 0666))){printf("Failed to create a FIFO File\n"); return 1;}

    printf("Created FIFO allllllllllrighttt\n");
    /* open file descriptor for FIFO*/

    if(-1 == (g_fd_named_pipe[1] = open(FIFO_set_a,O_WRONLY)))
    {
        printf("Failed to open pipe\n");
        return 1;
    }
    
    if(-1 == (g_fd_named_pipe[0] = open(FIFO_set_b,O_RDONLY)))
    {
        printf("Failed to open pipe\n");
        return 1;
    }
    #endif


    /*Excersice - 2*/
    #if 0
    /*Sem Inits*/
    if (SEM_FAILED == (pong_for_the = sem_open(name, O_CREAT ,0666 , 1))) 
    {
        perror("Sem no Buenu\n");
        return 1;
    }
    
    if (SEM_FAILED == (for_the_ping = sem_open(name1, O_CREAT ,0666 , 1))) 
    {
        perror("Sem no Buenu\n");
        return 1;
    }
        #endif

    /* Excercise 4*/
    #if 0
    printf("prior to struct init \n");

    mq_struct.mq_maxmsg = 10;
    mq_struct.mq_msgsize = 10;

    printf("after setting values in the struct \n");

    if(-1 == (mqd_iso = mq_open(mq_name,O_CREAT | O_WRONLY,0777,&mq_struct))){perror("mqd_iso failed to open"); return -1;}
    
    printf("after mq_open func \n");

    mq_getattr(mqd_iso,&mq_struct);
    if(10 != mq_struct.mq_msgsize){printf("this size is not 10");}
    
    #endif

    /* Excercise 5*/
    #if 1
    /*opened shared mem*/
    if(0 > (shm_fd = shm_open(shm_name,O_CREAT | O_EXCL | O_RDWR , 0700))){perror("shm_open");}
    /* make the file page size, and "initializing it" 
    either by zeros all the way, or to truncate the overflow */
    if(-1 == (ftruncate(shm_fd,getpagesize()))){perror("shm_ftruncate");}
    /*aquire the correct size of the shared memory from the inod*/
    if(-1 == fstat(shm_fd, &shm_inod)){perror("fstat");}
    /* mapping the shared memory to this current VM*/
    shm_ptr = (char *)mmap(NULL,shm_inod.st_size,PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd,0);
    
    #endif

}

void Eradicate()
{
    sem_close(pong_for_the);
    sem_unlink(name);
    sem_close(for_the_ping);
    sem_unlink(name1);
    close(g_fd_pipe_set_a[0]);
    close(g_fd_pipe_set_a[1]);
    close(g_fd_pipe_set_b[0]);
    close(g_fd_pipe_set_b[1]);
    waitpid(g_pid,NULL,__WALL);
    mq_getattr(mqd_iso,&mq_struct);
    if(0 == mq_struct.mq_curmsgs)
    {
        mq_unlink(mq_name);
    }
}


void ResetColor()
{
	printf("\033[0m");
}

void RedColor()
{
    printf ("\033[1;31m");
} 

void GreenColor()
{
	printf ("\033[1;32m");	
}
