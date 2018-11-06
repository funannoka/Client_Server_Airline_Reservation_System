/* TCPdaytimed.c - main */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
extern int errno;
int errexit(const char *format, ...);
int fileCopy(int fd);
int passiveTCP(const char *service, int qlen);
#define QLEN 5
/*------------------------------------------------------------------------
* main - Iterative TCP server for DAYTIME service
*------------------------------------------------------------------------
*/
int
main(int argc, char *argv[])
{
	struct sockaddr_in fsin; /* the from address of a client */
	char *service = "5500"; /* service name or port number */
	int msock, ssock; /* master & slave sockets */
	socklen_t alen; /* from-address length */
	switch (argc) {
		case 1:
			break;
		case 2:
			service = argv[1];
			break;
		default:
			errexit("usage: ./output [port]\n");
	}
	msock = passiveTCP(service, QLEN);
	while (1) {
		alen = sizeof(fsin);
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
		if (ssock < 0)
		errexit("accept failed: %s\n", strerror(errno));
		(void) fileCopy(ssock);
		(void) close(ssock);
	}
}
/*------------------------------------------------------------------------
* TCPdaytimed - do TCP DAYTIME protocol
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
