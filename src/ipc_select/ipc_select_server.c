/*********************************
 * Reviewer: Dani Pedrosa				
 * Author: Yohai Azoulai			
 * File: ipc_select.c					
 * ******************************/
 
 /**/

#include <assert.h>         /*assert*/
#include <stdlib.h>         /*malloc & free*/
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>


#include "ipc_select.h"
/*******Defines******/

#define PORT 5000
#define MAXLINE 1024
#define NO_USE(A)  ((void)(A))
#define MAX(a,b) ((a) > (b)) ? (a) : (b)

/*Excersice - */
#if 0

#endif

const char *message1 = "i'm not your friend guy";
const char *message2 = "i'm not your guy buddy";
const char *message3 = "i'm not your buddy friend";


/*******Structs******/
    
sa_in_t udp_client,tcp_client;

/*******Service Funcs******/



/*******Funcs Funcs******/


int main(int argc, char* argv[])
{
    int udp_fd = -1, tcp_listen_fd = -1 , accepted_fd = -1,  max_fd = 0;
    int run_fd = 0;
    socklen_t addr_size = sizeof(sa_in_t);
    fd_set read_set = {0}, master_fd_set = {0};
    uint16_t tcp_port = 0;
    uint16_t udp_port = 0;
    char output_buffer[BUFFSIZE] = {'\0'};
    
    if(3 > argc)
    {
        CheckError(STRPRNT,"Not enought arguments for func",
                            BRed,STRPRNT);
        CheckError(STRPRNT,"Argv1 is the TCP port",
                            BGreen,STRPRNT);
        CheckError(STRPRNT,"Argv2 is the UDP port",
                            BPurple,STRPRNT);

        exit(1);
    }
    
    if(3 <= argc)
    {
        tcp_port = atoi(argv[1]);
        udp_port = atoi(argv[2]);
    }
    else
    {
        udp_port = PORT_NUM;
        tcp_port = PORT_NUM + 1;
    }
    
    
    udp_fd = ServerSetup(udp_port, TCPBACKLOG, UDP,&udp_client);
    printf("%d - udp sockets ,%d - udp port\n",udp_fd,udp_port);
    tcp_listen_fd = ServerSetup(tcp_port, TCPBACKLOG, TCP,&tcp_client);
     
    printf("%d - tcp sockets %d tcp_sockets\n",tcp_listen_fd, tcp_port);


    CheckError(STRPRNT,"Server Up And Running",
                            On_IGreen,STRPRNT);
                            fflush(stdout);


    /*server creation 
        socket
        Bind
            TCP - Listen
            UDP -
            STDIN
        once thats initiated, continue to network loop*/

    FD_ZERO(&master_fd_set);
	FD_ZERO(&read_set);
	
    FD_SET(0, &master_fd_set);
	FD_SET(udp_fd, &master_fd_set);
	FD_SET(tcp_listen_fd, &master_fd_set);

	max_fd = MAX(udp_fd, tcp_listen_fd) + 1;

    while (1)
    {
        read_set = master_fd_set;
        if(-1 == (select(max_fd,&read_set,NULL,NULL,NULL))){perror("select");}

        for (run_fd = 0; !FD_ISSET(run_fd,&read_set) ; ++run_fd){}
        
            
            if(run_fd == udp_fd)
            {
                CheckError(STRPRNT,"tcp client sent messege",BIGreen,STRPRNT);
                Recive(run_fd,output_buffer,BUFFSIZE,0,&udp_client,&addr_size,SERVER);
                Send(run_fd,message1,0,&udp_client,&addr_size,SERVER);
            }
            if(run_fd == tcp_listen_fd || run_fd == accepted_fd)
            {
                
                if(run_fd == tcp_listen_fd)
                {
                    accepted_fd = AcceptNewConnect(tcp_listen_fd);
                    FD_SET(accepted_fd,&master_fd_set);
                    max_fd = MAX(max_fd, accepted_fd);
                }
    
                Recive(accepted_fd,output_buffer,BUFFSIZE,0,&tcp_client,&addr_size,SERVER);
                Send(accepted_fd,message2,0,&tcp_client,&addr_size,SERVER);
            }
            if(!run_fd)
            {
                fgets(output_buffer,BUFFSIZE,stdin);
                PrintColor(BIGreen);
                printf("%s\n",output_buffer);
                PrintColor(Color_Off);
                fflush(stdin);
            }
        
        
        
    }

    close(udp_fd);
	close(tcp_listen_fd);
	close(accepted_fd);

	return 0;

}





