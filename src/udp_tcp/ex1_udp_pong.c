/*********************************
 * Reviewer: Niko Hulkenberg				
 * Author: Yohai Azoulai			
 * File: ex3_udp_broad_pong.c					
 * ******************************/
 
 /*Client*/

#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/

#include "yoh_color.h"
#include "udp_tcp.h"
/*******Defines******/

char *message = "I'm not you Guy Buddy!\n";

/*******Structs******/

struct sockaddr_in servaddr;

/*******Service Funcs******/

char buffer[100];

/*******Funcs Funcs******/

int main(int argc, char **argv)
{
    size_t i = 0;
    int client_fd = 0;
    int add_length = 0;
    int add_length2 = sizeof(servaddr);
    if(argc < 2)
    {
        printf("Not enought arguments for func\n");
        exit(1);
    }
    int port = atoi(argv[1]);

    StructRest(&servaddr);

    client_fd = Creation(&servaddr,port,1,UDP);

    for (i = 0; i < 3; ++i)
    {
        Send(client_fd,message,0,&servaddr,&add_length2,1);
        /* code */
        Recive(client_fd,buffer,MAXLINE, 0,NULL,&add_length,1);
    }
    


    TerminateSocket(client_fd); 

    return 0;

}