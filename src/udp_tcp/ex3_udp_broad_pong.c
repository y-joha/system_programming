/* Client */

/*********************************
 * Reviewer: Niko Hulkenberg				
 * Author: Yohai Azoulai			
 * File: ex3_udp_broad_pong.c					
 * ******************************/
 
 /*Client*/

#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/
#include <stdio.h>
#include <string.h>


#include "udp_tcp.h"
/*******Defines******/

char *message = "I'm not your Guy Buddy!\n";
char *message1 = "I'm not your Friend Guy!\n";

/*******Structs******/

struct sockaddr_in servaddr;

/*******Service Funcs******/

char buffer[BUFFSIZE];

/*******Funcs Funcs******/

int main(int argc, char **argv)
{
    size_t i = 0;
    uint16_t port = 5000;
    int client_fd = 0;
    int add_length = 0;
    char *address_provided = NULL;
    int add_length2 = sizeof(servaddr);

    if(2 > argc)
    {
        printf("Not enought arguments for func\n");
        exit(1);
    }
    if(2 <= argc)
    {
        port = atoi(argv[1]);
    }
    else
    {
        port = PORT_NUM;
    }
    if(!argv[2]){address_provided = INADDR_ANY;}

    address_provided = (argv[2] ? argv[2] : INADDR_ANY);
 
    printf("client file ran??\n");
    StructRest(&servaddr);

    client_fd = UDPBroadCast(&servaddr, address_provided , port);
    for (i = 0; i < 3; ++i)
    {
        if(i == 1)
        {
            Send(client_fd,message1,0,&servaddr,&add_length2,CLIENT);
        }
        else
        {
            Send(client_fd,message,0,&servaddr,&add_length2,CLIENT);
        }
        printf("client sent now waiting to recive\n");
        Recive(client_fd,buffer,MAXLINE, 0,NULL,&add_length,CLIENT);
    }
    


    TerminateSocket(client_fd); 

    return 0;

}