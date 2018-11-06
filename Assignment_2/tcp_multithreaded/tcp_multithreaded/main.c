//
//  main.c
//  tcp_multithreaded
//
//  Created by Ifunanya Nnoka on 10/27/16.
//  Copyright © 2016 Ifunanya Nnoka. All rights reserved.
//

/* TCPechod.c - main, TCPechod */
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define QLEN 5 /* maximum connection queue length */
#define BUFSIZE 4096
extern int errno;
void reaper(int);
void *fileCopy(void *vargp);
int errexit(const char *format, ...);
int passiveTCP(const char *service, int qlen);
/*------------------------------------------------------------------------
 * main - Concurrent TCP server for ECHO service
 *------------------------------------------------------------------------
 */
int msock; /* master server socket */
int
main(int argc, char *argv[])
{
    char *service = "echo"; /* service name or port number */
    pthread_t tid;
    switch (argc) {
        case 1:
            break;
        case 2:
            service = argv[1];
            break;
        default:
            errexit("usage: TCPechod [port]\n");
    }
    msock = passiveTCP(service, QLEN);
    printf("%d\n",msock);
    while(1){
        pthread_create(&tid, NULL, fileCopy, (void*)msock);
        pthread_join(tid, NULL);
        
    }
    pthread_exit(NULL);
}
/*------------------------------------------------------------------------
 * TCPechod - echo data until end of file
 *------------------------------------------------------------------------
 */

void *fileCopy(void *vargp)
{
    int fd = (int) vargp;
    struct sockaddr_in fsin; /* the address of a client */
    socklen_t alen; /* length of client's address */
    char fromclient[256];
    char buff[1001];
    int n,n1;
    int ssock; /* slave server socket */
    FILE *source;
    alen = sizeof(fsin);
    printf("thread executing\n");
    ssock = accept(fd, (struct sockaddr *)&fsin, &alen);
    if (ssock < 0) {
        if (errno == EINTR)
            errexit("accept: %s\n", strerror(errno));
    }
    bzero(fromclient,256);
    /*Reading value from the client*/
    n = read(ssock,fromclient,255);
    bzero(buff,1001);
    printf("Request from client for file: %s\n",fromclient);
    if (n < 0)
        errexit("ERROR reading from socket %s\n", strerror(errno));
    else
    {
        fromclient[n] = '\0';
        if (fromclient[strlen(fromclient)-1] == '\n')
            fromclient[strlen(fromclient)-1] = '\0';
        if (fromclient[strlen(fromclient)-1] == '\r')
            fromclient[strlen(fromclient)-1] = '\0';
        
        if((source = fopen(fromclient, "r"))==NULL){
            bzero(buff,1001);
            *buff = '1';
        }
        else{
            fread(buff,sizeof(char),sizeof(buff),source);
        }
        n1= send(ssock, buff, sizeof(buff), 0);
        if (n1 <= 0)
        {
            errexit("ERROR writing to socket\n");
        }
        
    }
    (void) close(ssock);
   return NULL;
}
