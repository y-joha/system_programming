/*********************************
 * Reviewer: Dani Pedrosa				
 * Author: Yohai Azoulai			
 * File: ipc_select.c					$(PROG)
 * ******************************/
 
 /**/

#include <assert.h>     /*assert*/
#include <errno.h>      /*errno*/
#include <netinet/in.h> /*inet */
#include <string.h>     /*strlen*/
#include <strings.h>    /*bzero*/
#include <sys/socket.h> /*socket*/
#include <unistd.h>     /*bzero*/
#include <sys/select.h> /*select*/

#include "ipc_select.h"
/*******Defines******/

#define PORT 5000
#define MAXLINE 1024
#define NO_USE(A)  ((void)(A))

/*Excersice - */
#if 0

#endif

/*******Structs******/


/*******Service Funcs******/



/*******Funcs Funcs******/



int AcceptNewConnect(int server_socket)
{
    int client_sockt = 0;
    /*these are zero because only the server accepts new connections */
    CheckError(client_sockt = accept(server_socket,0,0),"Accept Func Failed",BIRed,ERR);

    return client_sockt;
}

int ServerSetup(int port, int backlog,int protocol,sa_in_t *addr_struc)
{
    int server_socket = 0;
    int res_prot = 0;

    res_prot = (protocol == UDP) * SOCK_DGRAM + (protocol == TCP) * SOCK_STREAM;
    
    CheckError(
        server_socket = socket(AF_INET,res_prot,0),
                        "Failed to Create Socket",
                        Red,
                        ERR);
    /*Server Specific*/
    
    addr_struc->sin_family = AF_INET;
    addr_struc->sin_port = htons(port);
    addr_struc->sin_addr.s_addr = htonl(INADDR_ANY);

    /*Binding The server */
    CheckError(STRPRNT,"Binding The Server",BIYellow,STRPRNT);
    
    CheckError(
    (bind(server_socket,(sa_t *)addr_struc,sizeof(sa_in_t))),
                    "Failed to Bind Server",
                    BIBlue,ERR);
    CheckError(STRPRNT,"Bind Complete",BIGreen,STRPRNT);
    
    /*If TCP Need Listen too */
    if(protocol == TCP)
    {
        CheckError(
                    listen(server_socket,backlog),
                    "Failed to Listen on FD TCP Server",
                    BIYellow,ERR);
        CheckError(STRPRNT,"TCP Server Lisetning on FD",BIGreen,STRPRNT);
    }

    return server_socket;
}

int ClientSetup(int port,const char * address_to_convert, int protocol,sa_in_t *addr_struc)
{
    int client_socket = 0;
    int res_prot = 0;

    res_prot = (protocol == UDP) * SOCK_DGRAM + (protocol == TCP) * SOCK_STREAM;
    
    CheckError(
        client_socket = socket(AF_INET,res_prot,0),
                        "Failed to Create Socket",
                        Red,
                        ERR);
    /*Server Specific*/
    addr_struc->sin_family = AF_INET;
    addr_struc->sin_port = htons(port);
    addr_struc->sin_addr.s_addr = inet_addr(address_to_convert);
    if(TCP == protocol){
        CheckError(
            connect(client_socket,(const sa_t *)addr_struc,sizeof(sa_t)),
                            "Failed to Create Socket",
                            Red,
                            ERR);
    }

    return client_socket;
}

void Recive(int listen_fd,char * buffer,size_t buffer_size, int flags, 
                        struct sockaddr_in * src_addr, socklen_t *addrlen, int is_client)
{
    ssize_t last_bit = 0;
    if(SERVER == is_client)
    {
        if(-1 == (last_bit = recvfrom(listen_fd,buffer,buffer_size,flags,(sa_t *)src_addr,addrlen))){perror("Server called recvfrom");}
        buffer[last_bit] = '\0';
    }
    else
    {
        if(-1 == (last_bit = recvfrom(listen_fd,buffer,buffer_size,flags,NULL,NULL))){perror("UDP Client called recvfrom");};
        buffer[last_bit] = '\0';
    }
    PrintColor(BGreen);
    printf("%s",buffer);
    PrintColor(Color_Off);
}

void Send(int listen_fd,const char * buffer, int flags, 
                        struct sockaddr_in * src_addr, socklen_t *addrlen, int is_client)
{
    socklen_t tmp = *addrlen;
    if(SERVER == is_client)
    {
        if(-1 == sendto(listen_fd,buffer,strlen(buffer)+1,flags,(const sa_t *)src_addr,tmp)){perror("sendto - Server");}
    }
    else
    {
        if(-1 == sendto(listen_fd,buffer,strlen(buffer)+1,flags, (const sa_t *)src_addr, tmp)){perror("sendto - Client");}
    }
}