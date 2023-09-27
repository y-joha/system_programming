/*********************************
 * Reviewer: Niko Hulkenberg				
 * Author: Yohai Azoulai			
 * File: ex2_udp_pong.c					
 * ******************************/
 
 /*CLIENT*/

#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/
#include <string.h>

#include "yoh_color.h"
#include "udp_tcp.h"
/*******Defines******/

char *message = "I'm not you Buddy friend!\n";

/*******Structs******/

struct sockaddr_in servaddr;

/*******Service Funcs******/

char buffer[100];

/*******Funcs Funcs******/

int main(int argc, char **argv)
{
    size_t i = 0;
    int server_fd = 0;
    int connection_fd = 0;
    int address_length = sizeof(struct sockaddr_in);
    if(argc < 2)
    {
        printf("Not enought arguments for func\n");
        exit(1);
    }
    int port = atoi(argv[1]);
    
    StructRest(&servaddr);

/*     server_fd = Creation(&servaddr,port,1,TCP);
 */

    while (1)
    {
        printf("writing loop - client\n");
        write(server_fd,message, 30);
        read(server_fd,buffer, 30);
        PrintColor(BIYellow);
        printf("%s\n",buffer);
        PrintColor(Color_Off);
    }

    close(connection_fd);
    close(server_fd);

    return 0;

}