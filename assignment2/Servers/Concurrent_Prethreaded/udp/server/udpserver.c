
#include <unistd.h> 
#include <stdio.h>
#include<errno.h>
extern int errno;
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#define __USE_BSD 1
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h> 
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
typedef void sigfunc (int);
#undef socklen_t
//#include "unp.h"
#define MAXN 16384  /* max # bytes client can request */
#define LISTENQ 50
#define MAXLINE 2048
int errexit(const char *format, ...);
int static nchildren;
static pid_t *pids;
FILE *fp;

/*---------------Function declarations------------------*/
int pthread_mutex_lock(pthread_mutex_t *mptr);
int pthread_mutex_unlock(pthread_mutex_t *mptr);
int pthread_create(pthread_t *tid, const pthread_attr_t *attr, void *(*func) (void *), void *arg);


/*-------------------Header that defines a thread structure that maintains info about each thread------------- */

typedef struct
{
 pthread_t thread_tid; /* thread ID */
 long thread_count; /* # connections handled */
 }thread;
 
 thread *tptr; /* array of Thread structures; calloc'ed */
 int listenfd, nthreads;
 socklen_t addrlen;
 pthread_mutex_t mlock;
 
/*-------------------------main---------------------------*/
 
 
 pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;
 
 int main(int argc, char **argv)
 {
 int i;
 void sig_int(int), thread_make(int);
if(argc = 3){
 listenfd = passiveUDP(argv[1]);
 nthreads = atoi(argv[argc - 1]);
}
else{
errexit("usage: serv07 [ <host> ] <port#> <#threads>");
}
 tptr = calloc(nthreads, sizeof(thread));
 for (i = 0; i < nthreads; i++)
 thread_make(i); /* only main thread returns */
 signal(SIGINT, sig_int);
for ( ; ; )
 pause(); /* everything done by threads */
 }

/*-------------------------thread make and thread main functions---------------*/

 
 void thread_make(int i)
 {
 void *thread_main(void *);
 pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *) i);
 return; /* main thread returns */
  }
  
  
 void * thread_main(void *arg)
 {
 int connfd;
 void web_child(int,char*);
 socklen_t clilen;
 struct sockaddr *cliaddr;
 cliaddr = malloc(addrlen);
 printf("thread %d starting\n", (int) arg);
 for ( ; ; ) 
 {
 clilen = addrlen;
 pthread_mutex_lock(&mlock);
// connfd = accept(listenfd, cliaddr, &clilen);
	struct sockaddr_in fsin; /* the from address of a client */
	char buffer[1001]; /* "input" buffer; any size > 0 */
	int n; /* from-address length */
	socklen_t alen;	
	bzero(buffer,1001);
	FILE *source;
	alen = sizeof(fsin);
	if ((n = recvfrom(listenfd,buffer,1001,0,(struct sockaddr *)&fsin, &addrlen))<0)
		errexit("recvfrom: %s\n", strerror(errno));

 pthread_mutex_unlock(&mlock);
 tptr[(int) arg].thread_count++;
 printf("%s",buffer);
 printf("%d",listenfd);
 web_child(n,buffer); 					/* process request */
 //close(connfd);
 }
 
 }
 
 
/*---------Signal Handler for SIG_INT------------------------------------*/

 void sig_int(int signo)
 {
 int i;
 void pr_cpu_time(void);
 /* terminate all children */
 
 for (i = 0; i < nchildren; i++)
 kill(pids[i], SIGTERM);
 while (wait(NULL) > 0) /* wait for all children */
 ;
 
 if(errno != ECHILD)
 errexit("wait error");
 
 pr_cpu_time();
 exit(0);
 }
 
 /*------------------wrapper function for---- Pthread_mutex_lock------------*/
 
 void Pthread_mutex_lock(pthread_mutex_t *mptr)
{
int n;
if((n=pthread_mutex_lock(mptr))==0)
return;
errno=n;
errexit("pthread_mutex_lock error");
}


/*------------------wrapper function for---- Pthread_mutex_unlock------------*/
 
 void Pthread_mutex_unlock(pthread_mutex_t *mptr)
{
int n;
if((n=pthread_mutex_unlock(mptr))==0)
return;
errno=n;
errexit("pthread_mutex_unlock error");
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

void web_child(int n,char* buffer)
 {
	struct sockaddr_in fsin; /* the from address of a client */
	socklen_t alen;	
	bzero(buffer,1001);
	FILE *source;
			buffer[n] = '\0';
		if (buffer[strlen(buffer)-1] == '\n')
			buffer[strlen(buffer)-1] = '\0';
		if (buffer[strlen(buffer)-1] == '\r')
			buffer[strlen(buffer)-1] = '\0';

		if((source = fopen(buffer, "r"))==NULL){
			bzero(buffer,1001);
			*buffer = '1';
		}
		else{			
			bzero(buffer,1001);
			n=fread(buffer,1001, 1,source);
		}
		buffer[strlen(buffer)]='\0';
		if ((n=sendto(listenfd,(char*) &buffer, strlen(buffer), 0,(struct sockaddr *)&fsin, addrlen))<0)
		{
			errexit("ERROR writing to socket: %s\n",strerror(errno));
		}
	
 }
