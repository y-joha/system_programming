/*********************************
 * Reviewer: I aint touching that with a 6ft. pole				
 * Author: Yohai Azoulai			
 * File: Pong_ping.c				
 * ******************************/


#include <sys/types.h>  /* fork - named-pipe*/
#include <sys/stat.h>   /*named-pipe*/
#include <sys/wait.h>   /* wait procc*/
#include <fcntl.h>      /*Sem Flags values*/
#include <unistd.h>     /* fork */
#include <stdio.h>      /* printf */
#include <string.h>     /* strlen*/
#include <semaphore.h>  /*semaphore*/
#include <mqueue.h>     /*message queue*/
#include <sys/mman.h>   /* mmap */




/*******Defines******/

int g_pid;
/* Excersice 2*/
#if 0
char *name  = "sam";
sem_t * pong_for_the = 0;
char *name1  = "I Am";
sem_t * for_the_ping = 0;
char *ping = "ping";
char *pong = "pong";
#endif
/* Excersice 4*/
#if 0
char *zaga = "zaga";
char *mq_name = "/messagequeue";
mqd_t mqd_sec = 0;
struct mq_attr mq_struct = {0};

#endif

/* Excersice 3*/
#if 0
char *FIFO_set_a = "IsoFile";
char *FIFO_set_b = "SecFile";
int g_fd_named_pipe[2];
int g_fd_pipe_set_a[2];
int g_fd_pipe_set_b[2];
#endif

/* Excersice 5*/
#if 1

int shm_fd;
char *shm_name = "Honky_Tonk";
char *shm_ptr;
struct stat shm_inod = {0};

#endif

char g_mila[20];
char g_shm_mila[10][20];

/*******Structs******/




/*******Service Funcs******/

int Init();
void Eradicate();
void ResetColor();
void RedColor();
void GreenColor();


/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    size_t i = 0;
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
        GreenColor();
        read(g_fd_named_pipe[0],g_mila,5);
        printf("%s - sec - Should be zaow\n",g_mila);
        ResetColor();
        write(g_fd_named_pipe[1],zaga,5);
        #endif

    }
    /*Excersice - 4*/
    #if 0

    GreenColor();
    if(-1 == mq_receive(mqd_sec,g_mila,10,NULL)){perror("mq_receive");}
    printf("%s - Reader \n",g_mila);
    ResetColor();

    #endif 

    /* Excercise 5*/
    #if 1
    /* copy from shared to buffer */
    /* for (i = 0; i < 5 ; ++i)
    {
        memcpy(g_shm_mila[i],shm_ptr,6);
    } */
    
    /*print*/
    for (i = 0 ; i < 5 ; ++i , shm_ptr += (strlen(shm_ptr) + 1))
    {
        printf("%s\n",(shm_ptr));
    }
    
    munmap(shm_ptr,shm_inod.st_size);
    
    #endif

    Eradicate();
 
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

    /* Excercise 3*/
    #if 0
    
    /* open the fifo file in secondry proccess in exc 3*/
    printf("Opening FIFO Named Pipe");
    
    if(-1 == (g_fd_named_pipe[0] = open(FIFO_set_a,O_RDONLY)))
    {
        printf("Failed to open pipe");
        return 1;
    }
    
    if(-1 == (g_fd_named_pipe[1] = open(FIFO_set_b,O_WRONLY)))
    {
        printf("Failed to open pipe");
        return 1;
    }
    #endif

    /*Excersice - 2*/
    #if 0
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

    if(-1 == (mqd_sec = mq_open(mq_name,O_RDONLY))){perror("mqd_sec failed to open"); return -1;}

    #endif

    /* Excercise 5*/
    #if 1
    if(0 > (shm_fd = shm_open(shm_name, O_RDONLY, 0666))){perror("shm_open failed to open");}
    /*aquire the correct size of the shared memory from the inod*/
    if(-1 == fstat(shm_fd, &shm_inod)){perror("fstat");}
    /* map to current location */
    shm_ptr = mmap(NULL,shm_inod.st_size,PROT_READ,MAP_SHARED,shm_fd,0);
    

    #endif

}

void Eradicate()
{
    #if 0
    waitpid(g_pid,NULL,__WALL);
    sem_close(pong_for_the);
    sem_unlink(name);
    sem_close(for_the_ping);
    sem_unlink(name1);
    close(g_fd_pipe_set_a[0]);
    close(g_fd_pipe_set_a[1]);
    close(g_fd_pipe_set_b[0]);
    close(g_fd_pipe_set_b[1]);
    mq_getattr(mqd_sec,&mq_struct);
    if(0 == mq_struct.mq_curmsgs)
    {
        mq_unlink(mq_name);
    }
    #endif
    shm_unlink(shm_name);
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


/*Excersice - */
#if 0

#endif