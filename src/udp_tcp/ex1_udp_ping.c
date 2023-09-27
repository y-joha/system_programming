/*********************************
 * Reviewer: Niko Hulkenberg				
 * Author: Yohai Azoulai			
 * File: ex1_udp_ping.c					
 * ******************************/
 
 /*SERVER*/

#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/
#include <string.h>


#include "udp_tcp.h"
/*******Defines******/

char *message = "I'm not you friend Guy!\n";

/*******Structs******/

struct sockaddr_in servaddr, cliaddr;

/*******Service Funcs******/

char buffer[100];

/*******Funcs Funcs******/

int main(int argc, char **argv)
{
    size_t i = 0;
    int server_fd = 0;
    int address_length = 0;
    if(argc < 2)
    {
        printf("Not enought arguments for func\n");
        exit(1);
    }
    int port = atoi(argv[1]);
    StructRest(&servaddr);
    StructRest(&cliaddr);

    server_fd = Creation(&servaddr,port,0,UDP);

    for (i = 0; i < 3; ++i)
    {
        Recive(server_fd,buffer,sizeof(buffer),0,&cliaddr,&address_length,0);

        Send(server_fd,message,0,&cliaddr,&address_length,0);
    }
    


    return 0;

}