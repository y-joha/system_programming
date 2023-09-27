/*********************************
 * Reviewer: Niko Hulkenberg				
 * Author: Yohai Azoulai			
 * File: ex3_udp_broad_ping.c					
 * ******************************/
 
 /*SERVER*/

#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/
#include <string.h>

#include "udp_tcp.h"
/*******Defines******/

char *message = "I'm not your Buddy Friend!\n";

/*******Structs******/

struct sockaddr_in servaddr, cliaddr;

/*******Service Funcs******/

char buffer[BUFFSIZE];

/*******Funcs Funcs******/

int main(int argc, char **argv)
{
    size_t i = 0;
    uint16_t port = 0;
    int server_fd = 0;
    int address_length = 0;
    char *address_provided = NULL;
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
    
    address_provided = (argv[2] ? argv[2] : INADDR_ANY);
    StructRest(&servaddr);
    StructRest(&cliaddr);

    server_fd = Creation(&servaddr, address_provided, port,SERVER,UDP);

    for (i = 0; i < 3; ++i)
    {
        Recive(server_fd,buffer,sizeof(buffer),0,&servaddr,&address_length,0);

        Send(server_fd,message,0,&servaddr,&address_length,SERVER);
    }
    
    return 0;

}