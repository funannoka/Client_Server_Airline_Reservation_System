/* UDPtimed.c - main */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
typedef unsigned long u_long;
extern int errno;
sem_t sock_lock;

void *thread_function(void *vargp);
int passiveUDP(const char *service);
int errexit(const char *format, ...);
/*------------------------------------------------------------------------
* main - Iterative UDP server for TIME service
*------------------------------------------------------------------------
*/
int
main(int argc, char *argv[])
{
	pthread_t tid;
	char *service = "5500"; /* service name or port number */
	int sock;	
	sem_init(&sock_lock,0,1);
	switch (argc) {
		case 1:
			break;
		case 2:
			service = argv[1];
			break;
		default:
			errexit("usage: UDPtimed [port]\n");
	}
	sock = passiveUDP(service);
while(1){
	pthread_create(&tid, NULL, thread_function, (void *) sock);
	pthread_join(tid,NULL);
}				
pthread_exit(NULL);
(void) close(sock);
}

void *thread_function(void *vargp){
	struct sockaddr_in fsin; /* the from address of a client */
	char buffer[1001]; /* "input" buffer; any size > 0 */
	int sock=(int*) vargp; /* server socket */
	int n; /* from-address length */
	socklen_t alen;	
	bzero(buffer,1001);
	FILE *source;
	alen = sizeof(fsin);
	sem_wait(&sock_lock);

	if ((n = recvfrom(sock,buffer,1001,0,(struct sockaddr *)&fsin, &alen))<0)
		errexit("recvfrom: %s\n", strerror(errno));
	else 
	{
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
	
		//n= write(sock, (char*)&buffer, strlen(buffer));
		if ((n=sendto(sock,(char*) &buffer, strlen(buffer), 0,(struct sockaddr *)&fsin, sizeof(fsin)))<0)
		{
			errexit("ERROR writing to socket: %s\n",strerror(errno));
		}
	}
	sem_post(&sock_lock);
}
