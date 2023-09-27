/*********************************
 * Reviewer: Dani Pedrosa				
 * Author: Yohai Azoulai			
 * File: ipc_select.c					
 * ******************************/

#ifndef __IPC_SELECT_H__
#define __IPC_SELECT_H__

#include "yoh_color.h"
#include <arpa/inet.h>  /*inet_addr*/
#include <unistd.h>     /*read write*/

#define PORT_NUM 5000
#define BUFFSIZE 100
#define TCPBACKLOG 100
#define SOCKERR (-1)
#define HOM_ADDR "127.0.0.1"


typedef struct sockaddr sa_t;
typedef struct sockaddr_in sa_in_t;

enum connection_role {SERVER,CLIENT};

enum connection_protocol {UDP,TCP};

int AcceptNewConnect(int server_socket);
int ServerSetup(int port,int backlog,int protocol, sa_in_t *addr_struc);
int ClientSetup(int port,const char * address_to_use,int protocol, sa_in_t *addr_struc);
void Recive(int listen_fd,char * buffer,size_t buffer_size, int flags, 
                        struct sockaddr_in * src_addr, socklen_t *addrlen, int is_client);
void Send(int listen_fd,const char * buffer, int flags, 
                        struct sockaddr_in * src_addr, socklen_t *addrlen, int is_client);

#endif 
