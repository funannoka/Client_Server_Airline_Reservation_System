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
typedef unsigned long u_long;
extern int errno;
int passiveUDP(const char *service);
int errexit(const char *format, ...);
/*------------------------------------------------------------------------
* main - Iterative UDP server for TIME service
*------------------------------------------------------------------------
*/
int
main(int argc, char *argv[])
{
	struct sockaddr_in fsin; /* the from address of a client */
	char *service = "time"; /* service name or port number */
	char buffer[1001]; /* "input" buffer; any size > 0 */
	int sock; /* server socket */
	int n; /* from-address length */
	socklen_t alen;	
	bzero(buffer,1001);
	FILE *source;
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
	while (1) {
		alen = sizeof(fsin);
		if ((n = recvfrom(sock,buffer,1001,0,(struct sockaddr *)&fsin, &alen))<0)
//read(sock, (char *)&buffer, 1001)) < 0)
			errexit("recvfrom: %s\n", strerror(errno));
		else 
		{
		switch (fork()) {
			case 0: /* child */
//				(void) close(msock);
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
			default: /* parent */
//				(void) close(ssock);
				break;
			case -1:
				errexit("fork: %s\n", strerror(errno));
		}
					
		}
		//(void) sendto(sock, (char *)&now, sizeof(now), 0,(struct sockaddr *)&fsin, sizeof(fsin));
	}
}

