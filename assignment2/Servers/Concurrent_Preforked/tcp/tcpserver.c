//#include "unp.h"
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<stdlib.h>
#include<signal.h>
#include <sys/socket.h> /*Defines functions used to perform socket related operations*/
#include <unistd.h> /*Defines constants type and function declarations that overrides POSIX OS API*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h> /*Defines constant and structures needed for Internet domain addresses*/
#include <arpa/inet.h> /*Definations for internet options*/
#include <netdb.h> /*Defines Network Database operations*/
#include<time.h>
#include<stdarg.h>
#include <syslog.h>
#define LISTENQ 10
int static nchildren;
static pid_t *pids;

int main(int argc, char **argv)
{

 int listenfd, i;
 socklen_t addrlen;
 void sig_int(int);
 pid_t child_make(int, int, int);

 if (argc == 3)
    listenfd =tcp_listen(NULL, argv[1], &addrlen);
 else if (argc == 4)
    listenfd = tcp_listen(argv[1], argv[2], &addrlen);
 else
    errexit("usage: serv02 [ <host> ] <port#> <#children>");
    nchildren = atoi(argv[argc - 1]);
    pids = calloc(nchildren, sizeof(pid_t));

 for (i = 0; i < nchildren; i++)
    pids[i] = child_make(i, listenfd, addrlen); /* parent returns */
 signal(SIGINT, sig_int);

for ( ; ; )

pause();

/* everything done by children */

}

/*
Figure 30.10 Singal handler for SIGINT.
*/

void sig_int(int signo)
{
     int     i;
     void    pr_cpu_time(void);

         /* terminate all children */
     for (i = 0; i < nchildren; i++)
         kill(pids[i], SIGTERM);
     while (wait(NULL) > 0)     /* wait for all children */
         ;
     if (errno != ECHILD)
         errexit("wait error");

     pr_cpu_time();
     exit(0);
}

/*
Figure 30.11 child_make function: creates each child.

server/child02.c

 1 #include    "unp.h"
*/

pid_t child_make(int i, int listenfd, int addrlen)
  {
      pid_t   pid;
      void    child_main(int, int, int);

      if ( (pid = fork()) > 0)
          return (pid);            /* parent */

      child_main(i, listenfd, addrlen);     /* never returns */
 }

/*
Figure 30.12 child_main function: infinite loop executed by each child.

server/child02.c
*/
 void child_main(int i, int listenfd, int addrlen)
 {
     int     connfd;
     void    web_child(int);
     socklen_t clilen;
     struct sockaddr *cliaddr;

     cliaddr = malloc(addrlen);

     printf("child %ld starting\n", (long) getpid());
     for ( ; ; ) {
         clilen = addrlen;
         connfd = accept(listenfd, cliaddr, &clilen);

         web_child(connfd);      /* process the request */
         close(connfd);
     }
}

#include    <sys/resource.h>
#ifndef HAVE_GETRUSAGE_PROTO
int     getrusage(int, struct rusage *);
#endif

void pr_cpu_time(void)
 {
      double user, sys;
     struct rusage myusage, childusage;

     if (getrusage(RUSAGE_SELF, &myusage) < 0)
         errexit("getrusage error");
     if (getrusage(RUSAGE_CHILDREN, &childusage) < 0)
         errexit("getrusage error");

     user = (double) myusage.ru_utime.tv_sec +
         myusage.ru_utime.tv_usec / 1000000.0;
     user += (double) childusage.ru_utime.tv_sec +
         childusage.ru_utime.tv_usec / 1000000.0;
     sys = (double) myusage.ru_stime.tv_sec +
         myusage.ru_stime.tv_usec / 1000000.0;
     sys += (double) childusage.ru_stime.tv_sec +
         childusage.ru_stime.tv_usec / 1000000.0;

     printf("\nuser time = %g, sys time = %g\n", user, sys);
 }

  #define MAXLINE 10000
  #define MAXN    16384     /* max # bytes client can request */

 void  web_child(int fd)
 {
     char fromclient[256];
	char buff[1001];
	int n,n1;
	FILE *source;
	if (fd < 0) 
		errexit("ERROR on accept");
	bzero(fromclient,256);
	/*Reading value from the client*/
	n = read(fd,fromclient,255);
	bzero(buff,1001);
	if (n < 0) errexit("ERROR reading from socket");
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
		fread(buff,1001, 1,source);
		}
			n1= send(fd, buff, sizeof(buff), 0);
			   if (n1 <= 0)
				{
					errexit("ERROR writing to socket\n");
				}
	
	}
 }

 int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
 {
   int listenfd, n;
   const int on = 1;
   struct addrinfo hints, *res, *ressave;
   bzero(&hints, sizeof (struct addrinfo)) ;
   hints.ai_flags = AI_PASSIVE;
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   if ( (n = getaddrinfo (host, serv, &hints, &res)) != 0)
   errexit("tcp_listen error for %s, %s: %s",host, serv, gai_strerror(n)) ;
   ressave = res;
   do {
  listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (listenfd < 0)
  continue; /* error, try next one */
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on) ) ;
  if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
  break; /* success */
  close (listenfd); /* bind error, close and try next one */
 }while ( (res = res->ai_next) != NULL);
   if (res == NULL) /* errno from final socket () or bind () */
      errexit ("tcp_listen error for %s, %s", host, serv);
       listen (listenfd, LISTENQ);
    if (addrlenp)
      *addrlenp = res->ai_addrlen; /* return size of protocol address */
      freeaddrinfo (ressave);
     return (listenfd);

 }

