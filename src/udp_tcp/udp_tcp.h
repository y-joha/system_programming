/*********************************
 * Reviewer: Niko Hulkenberg				
 * Author: Yohai Azoulai			
 * File: udp.c					
 * ******************************/

#ifndef __UDP_TCP_H__
#define __UDP_TCP_H__

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define STRUCTADDRESS struct sockaddr
#define MAXLINE 1000
#define PORT_NUM 5000
#define HOME_ADDR "127.0.0.1"
#define BUFFSIZE 1024

enum server_client {SERVER, CLIENT};

enum protocol
{
    UDP,
    TCP
};



void StructRest(struct sockaddr_in * server_struct);

int Creation(
        struct sockaddr_in * server_struct,
        const char * address , 
        int port, 
        int is_client,
        int protocol);


void Recive(
        int listen_fd,
        char * buffer,
        size_t buffer_size, 
        int flags, 
        struct sockaddr_in * src_addr, 
        socklen_t *addrlen, 
        int waiting_responce);

void Send(
        int listen_fd,
        char * buffer, 
        int flags, 
        struct sockaddr_in * src_addr, 
        socklen_t *addrlen,int is_client);
                        
void TerminateSocket(int socket_fd);

int UDPBroadCast(
        struct sockaddr_in *server_struct, 
        char *address, 
        int port);

int TCPAccept(
        int client_fd, 
        struct sockaddr_in * src_addr, 
        socklen_t *addrlen);


#endif 
