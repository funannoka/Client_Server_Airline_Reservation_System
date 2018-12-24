#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <netinet/in.h>
#include "encrypt.h"
extern int errno;
int TCPclient(const char *host,const char *service);
int errexit(const char *format, ...);
int connectTCP(const char *host, const char *service);
/*------------------------------------------------------------------------
 * main - TCP client for File server
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
    char *host = "localhost";
    char *service = "5500"; /* default service port */
    if(argc!=3){
        printf("Please run with these command line inputs [host [port]]\n");
        exit(0);
    }
    switch (argc) {
        case 3:
            service = argv[2];
            /* FALL THROUGH */
        case 2:
            host = argv[1];
            break;
        default:
            fprintf(stderr, "Please run with these command line inputs [host [port ]]\n");
            exit(1);
    }
    TCPclient(host, service);
    exit(0);
}
/*------------------------------------------------------------------------
 * Client - invoke on specified host and print results
 *------------------------------------------------------------------------
 */
int TCPclient(const char *host, const char *service)
{
    
    int s1,n; /* socket, read count */
    char *buffer="hello";
    char server_buffer[2701];
    char *encrypt;
    char *decrypt;
    /*Client connecting to server: */
    s1 = connectTCP(host, service);
    //fgets(buffer,1000,stdin);
    strcpy(server_buffer,buffer);
    encrypt = Encrypt(server_buffer);
    while(s1){
    write:	n = write(s1,encrypt,strlen(encrypt));
        if (n < 0) {
            errexit("ERROR writing to socket %s\n", strerror(errno));
        }
    read:   bzero(server_buffer,2701);
        if((n=read(s1,server_buffer,2701)) > 0){
            decrypt=Decrypt(server_buffer);
            bzero(server_buffer,sizeof(server_buffer));
            strcpy(server_buffer,decrypt);
            printf("%s\n",server_buffer);
            server_buffer[n] = '\0';
            if (server_buffer[strlen(server_buffer)-1] == '\n')
                server_buffer[strlen(server_buffer)-1] = '\0';
            if(strcmp(server_buffer,"Goodbye!")==0){
                strcpy(server_buffer,buffer);
                goto write; //exit from current service module
            }
            if(strcmp(server_buffer,"Have a Wonderful day!!!!")==0){
                return 0;
            }
            bzero(server_buffer,2701);
            printf(":::: >>>>  ");
            fgets(server_buffer,2701,stdin);
            if(server_buffer[strlen(server_buffer) - 1] == '\n')
                server_buffer[strlen(server_buffer) - 1] = '\0';
            encrypt = Encrypt(server_buffer);
        }else{
            printf("Error readng from the socket\n");
            return 0;
        }
    }
    return 0;
}

