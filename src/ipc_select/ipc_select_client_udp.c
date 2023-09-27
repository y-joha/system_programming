/*********************************
 * Reviewer: Dani Pedrosa				
 * Author: Yohai Azoulai			
 * File: ipc_select.c					
 * ******************************/
 
 /**/


#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/
#include <string.h>

#include "ipc_select.h"
/*******Defines******/

char *message = "I'm not you Guy Buddy!\n";

/*******Structs******/

struct sockaddr_in udp_server;

/*******Service Funcs******/

char buffer[BUFFSIZE];

/*******Funcs Funcs******/

int main(int argc, char **argv)
{
    size_t i = 0;
    int client_fd = 0;
    socklen_t add_length = 0;
    int port = 0;
    socklen_t add_length2 = sizeof(udp_server);

    if(argc < 2)
    {
        printf("Not enought arguments for func\n");
        exit(1);
    }
    if(2 > argc)
    {
        CheckError(STRPRNT,"Not enought arguments for func",
                            BRed,STRPRNT);
        CheckError(STRPRNT,"Argv1 is the UDP port",
                            BGreen,STRPRNT);
        CheckError(STRPRNT,"Address is 127.0.0.1 hardcoded",
                            BPurple,STRPRNT);

        exit(1);
    }

    port = atoi(argv[1]);

    
    client_fd = ClientSetup(port,HOM_ADDR,UDP,&udp_server);


    
    for (i = 0; i < 3; ++i)
    {
/*         Send(client_fd,message,0,&udp_server,&add_length2,CLIENT);
 */        
        if(-1 == sendto(client_fd,message,strlen(message)+1,MSG_CONFIRM, (const sa_t *)&udp_server, add_length2)){perror("sendto - Client");}
        printf("Client sent payload");
        Recive(client_fd,buffer,BUFFSIZE, MSG_WAITALL, NULL,&add_length,CLIENT);
        printf("Client recevied payload");
    }

/*     (void)ProggressBar;
 */    (void)CheckError;

    close(client_fd);

    return 0;

}