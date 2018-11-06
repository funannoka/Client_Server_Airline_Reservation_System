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

#define QLEN 5 /* maximum connection queue length */
#define BUFSIZE 4096
extern int errno;
void reaper(int);
int fileCopy(int fd);
int errexit(const char *format, ...);
int passiveTCP(const char *service, int qlen);
/*------------------------------------------------------------------------
* main - Concurrent TCP server for ECHO service
*------------------------------------------------------------------------
*/
int
main(int argc, char *argv[])
{
	char *service = "5500"; /* service name or port number */
	struct sockaddr_in fsin; /* the address of a client */
	socklen_t alen; /* length of client's address */
	int msock; /* master server socket */
	int ssock; /* slave server socket */
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
	(void) signal(SIGCHLD, reaper);
	while (1) {
		alen = sizeof(fsin);
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
		if (ssock < 0) {
			if (errno == EINTR)
			continue;
			errexit("accept: %s\n", strerror(errno));
		}
		switch (fork()) {
			case 0: /* child */
				(void) close(msock);
				exit(fileCopy(ssock));
			default: /* parent */
				(void) close(ssock);
				break;
			case -1:
				errexit("fork: %s\n", strerror(errno));
		}
	}
}
/*------------------------------------------------------------------------
* TCPechod - echo data until end of file
*------------------------------------------------------------------------
*/
int
fileCopy(int fd)
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
	return 0;
}
/*------------------------------------------------------------------------
* reaper - clean up zombie children
*------------------------------------------------------------------------
*/
/*ARGSUSED*/
void
reaper(int sig)
{
	int status;
	while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0)
		/* empty */;
}
