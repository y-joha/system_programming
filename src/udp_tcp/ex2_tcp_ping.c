/*********************************
 * Reviewer: Niko Hulkenberg				
 * Author: Yohai Azoulai			
 * File: ex2_udp_ping.c					
 * ******************************/
 
 /*SERVER*/

#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/
#include <string.h>

#include "yoh_color.h"
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
    int connection_fd = 0;
    int address_length = sizeof(cliaddr);
    if(argc < 2)
    {
        printf("Not enought arguments for func\n");
        exit(1);
    }
    int port = atoi(argv[1]);
    
    StructRest(&servaddr);
    StructRest(&cliaddr);

    server_fd = Creation(&servaddr,port,0,TCP);

    connection_fd = TCPAccept(server_fd,&cliaddr,&address_length);

    while (1)
    {
        
        printf("writing loop - server\n");
        read(connection_fd,buffer, 30);
        PrintColor(Blue);
        printf("%s\n",buffer);
        PrintColor(Color_Off);
        write(connection_fd,message, 30);
    }

    close(connection_fd);
    close(server_fd);

    return 0;

}