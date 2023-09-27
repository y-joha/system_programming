/*********************************
 * Reviewer: Niko Hulkenberg				
 * Author: Yohai Azoulai			
 * File: udp_tcp.c					
 * ******************************/
 
 /**/

#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/

#include "yoh_color.h"
#include "udp_tcp.h"
/*******Defines******/



/*******Structs******/



/*******Service Funcs******/

void ResetColor();
void RedColor();
void GreenColor();

/*******Funcs Funcs******/

void StructRest(struct sockaddr_in *server_struct)
{
    bzero(&server_struct, sizeof(server_struct));
}

int Creation(struct sockaddr_in *server_struct, const char *address, int port, int is_client, int protocol)
{
    int fd = 0;
    int prot = 0;
    int addr_len = sizeof(struct sockaddr_in);
    
    prot = (protocol == UDP) * SOCK_DGRAM + (protocol == TCP) * SOCK_STREAM;
    
    PrintColor(On_IYellow);
    printf("Pre - Socket\n");
    PrintColor(Color_Off);
    
    if(-1 == (fd = socket(AF_INET, prot, 0))){perror("socket\n"); return -1;}
    
    
    printf("Pst - Socket\n");
    
    
    server_struct->sin_family = AF_INET;
    server_struct->sin_port = htons(port);
    server_struct->sin_addr.s_addr = inet_addr(address);

    if(SERVER == is_client)
    {
        GreenColor();
        printf("Server - Binding\n");
        server_struct->sin_addr.s_addr = htonl(INADDR_ANY);
        if(-1 == (bind(fd,(STRUCTADDRESS *)server_struct,sizeof(struct sockaddr_in)))){perror("Bind failed!");};
        if(TCP == protocol)
        {
            if (0 != (listen(fd,10)))
            {
                RedColor();
                perror("listen (TCP) failed buddy!\n");
                ResetColor();
            }
            printf("server listening on %d port\n",port);
        }
        printf("Server - After Binding\n");
    }
    if(TCP == protocol && is_client)
    {
        GreenColor();
        printf("Client - Connecting\n");
        ResetColor();
        if(-1 == (connect(fd, (STRUCTADDRESS *)server_struct, addr_len))){perror("connect TCP failed!");};
        RedColor();
        printf("Client - Finished Connecting\n");
        ResetColor();
    }
    
    return fd;
}
    
int UDPBroadCast(
        struct sockaddr_in *server_struct, 
        char *address, 
        int port)
{
    int brodcast_fd = 0;
    int ret = 0;
    int broadcastEnable = 1;

    server_struct->sin_family = AF_INET;
    server_struct->sin_port = htons(port);
    server_struct->sin_addr.s_addr = htonl(INADDR_BROADCAST);
    printf("Bla bla before seocket UDP Broad\n");
    brodcast_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(brodcast_fd < 0)
    {
        perror("socket\n");
    }
    printf("Bla bla before setsockopt\n");
    if(-1 == setsockopt(brodcast_fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable))){perror("setsockopt - Failed");}
    printf("Bla bla before setsockopt\n");
    return brodcast_fd;
}


void Recive(int listen_fd,char * buffer,size_t buffer_size, int flags, 
                        struct sockaddr_in * src_addr, socklen_t *addrlen, int is_client)
{
    ssize_t last_bit = 0;
    if(SERVER == is_client)
    {
        *addrlen = sizeof(struct sockaddr_in);
        if(-1 == (last_bit = recvfrom(listen_fd,buffer,buffer_size,flags,(STRUCTADDRESS *)src_addr,addrlen))){perror("recvfrom");}
        buffer[last_bit] = '\0';
    }
    else
    {
        if(-1 == recvfrom(listen_fd,buffer,buffer_size,flags,NULL,NULL)){perror("recvfrom");};
        buffer[last_bit] = '\0';
    }
    PrintColor(BGreen);
    printf("%s",buffer);
    PrintColor(Color_Off);
}

void Send(int listen_fd,char * buffer, int flags, 
                        struct sockaddr_in * src_addr, socklen_t *addrlen, int is_client)
{
    if(0 == is_client)
    {
        if(-1 == sendto(listen_fd,buffer,strlen(buffer),0,(const STRUCTADDRESS *)src_addr,*addrlen)){perror("sendto - Server");}
    }
    else
    {
        if(-1 == sendto(listen_fd,buffer,strlen(buffer),0, (const STRUCTADDRESS *)src_addr, sizeof(struct sockaddr_in))){perror("sendto - Client");}
    }
}

int TCPConnect(int side, uint16_t port_num, int num_connections)
{
    int socket_fd = 0;

    return socket_fd;
}

int TCPAccept(int client_fd, struct sockaddr_in * src_addr, socklen_t *addrlen)
{
    int return_fd = 0;
    GreenColor();
    printf("Accept function\n");
    ResetColor();
    if(-1 == (return_fd = accept(client_fd, (STRUCTADDRESS *)src_addr, addrlen)))
    {
        perror("accept - Client failed\n");
    }
    RedColor();
    printf("Accept function Finished!!\n");
    ResetColor();
    return return_fd;
}

void TerminateSocket(int socket_fd)
{
    close(socket_fd);
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