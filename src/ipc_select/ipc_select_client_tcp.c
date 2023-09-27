/*********************************
 * Reviewer: Dani Pedrosa				
 * Author: Yohai Azoulai			
 * File: ipc_select.c					
 * ******************************/
 
 /**/

#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/
#include <string.h>
#include <sys/types.h>     /*socklen_t*/
#include <sys/socket.h>


#include "ipc_select.h"
/*******Defines******/

char *message = "I'm not you Buddy friend!\n";

/*******Structs******/

struct sockaddr_in tcp_client;

/*******Service Funcs******/

char buffer[BUFFSIZE];

/*******Funcs Funcs******/

int main(int argc, char **argv)
{
    int client_fd = 0;
    int port = 0;
    char *address_provided = NULL;
    if(argc < 3)
    {
        printf("Not enought arguments for func\n");
        exit(1);
    }
    if(3 > argc)
    {
        CheckError(STRPRNT,"Not enought arguments for func",
                            BRed,STRPRNT);
        CheckError(STRPRNT,"Argv1 is the TCP port",
                            BGreen,STRPRNT);
        CheckError(STRPRNT,"Argv2 is the required TCP address",
                            BPurple,STRPRNT);

        exit(1);
    }
    port = atoi(argv[1]);

    address_provided = (argv[2] ? argv[2] : INADDR_ANY);

    client_fd = ClientSetup(port,address_provided,TCP,&tcp_client);
    CheckError(STRPRNT,"TCP Client has recived fd",BIYellow,STRPRNT);
    while (1)
    {
        CheckError(write(client_fd,message, 30),"TCP Client Write Failed",BRed,ERR);
        CheckError(STRPRNT,"tcp client sent messege",BIGreen,STRPRNT);
        CheckError(read(client_fd,buffer, 30),"TCP Client Read Failed",BRed,ERR);
        CheckError(STRPRNT,"tcp client recevied messege",BICyan,STRPRNT);
        PrintColor(BIYellow);
        printf("%s\n",buffer);
        PrintColor(Color_Off);
    }

    close(client_fd);
/*     (void)ProggressBar;
*/  return 0;

}