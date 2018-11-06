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
		char *service = "5500"; /* service name or port number */
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
		//printf("%d\n",msock);
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
	void
	*fileCopy(void *vargp)
	{
		int fd = (int) vargp;	
		struct sockaddr_in fsin; /* the address of a client */
		socklen_t alen; /* length of client's address */
		char fromclient[256];
		char buff[1001];
		char *token,*token_bu;
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
		if (n < 0) 
			errexit("ERROR reading from socket %s\n", strerror(errno));
		else 
		{
			bzero(buff,1001);
			token = strtok(fromclient, " /");
			int i=0;
			while( token != NULL && i<1) 
			{
				token = strtok(NULL, " /");
				i++;
			}
			printf("Request from client for file: %s\n",token);
			n=strlen(token);		
			token[n] = '\0';
			if (token[strlen(token)-1] == '\n')
				token[strlen(token)-1] = '\0';
			if (token[strlen(token)-1] == '\r')
				token[strlen(token)-1] = '\0';
			bzero(buff,1001);
			if((source = fopen(token, "r"))==NULL){
				strcat(buff, "HTTP/1.1 404 Not Found\n");
				strcat(buff, "Connection: Close\n");
				strcat(buff, "Content-length: 1001\n");
				strcat(buff, "Content-Type: text/html\n\n");
				strcat(buff,"<html><body><H1>404 File Not Found</H1></body></html>");
				//printf("socket content: %s\n",buff);				
				n1=write(ssock,buff ,strlen(buff));
				   if (n1 <= 0)
					{
						errexit("ERROR writing to socket\n");
					}
			}
			else{
				bzero(fromclient,1001);
				fread(buff,1001, 1,source);
				strcat(fromclient, "HTTP/1.1 200 OK\n");
				strcat(fromclient, "Connection: Keep-Alive\n");
				strcat(fromclient, "Content-length: 1001\n");
				strcat(fromclient, "Content-Type: text/html\n\n");
				strcat(fromclient,"<html><body><H1>");
				strcat(fromclient,buff);
				strcat(fromclient,"</H1></body></html>");
				n1=write(ssock, fromclient,strlen(fromclient));
				   if (n1 <= 0)
					{
						errexit("ERROR writing to socket\n");
					}
			}

}
	void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}
		(void) close(ssock);
	}
