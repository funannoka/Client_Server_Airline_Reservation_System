/* TCPdaytime.c - TCPdaytime, main */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <netinet/in.h>
extern int errno;
int TCPtime(const char *host,const char *service,const char *filename);
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
	char *filename = "";
	if(argc!=4){
		printf("Please run with these command line inputs [host [port [filename to be copied]]]\n");
		exit(0);
	}
	switch (argc) {
		case 4: filename = argv[3];
			/*FALL THROUGH*/
		case 3:
			service = argv[2];
			/* FALL THROUGH */
		case 2:
			host = argv[1];
			break;
		default:
			fprintf(stderr, "Please run with these command line inputs [host [port [filename to be copied]]]\n");
			exit(1);
	}
	TCPtime(host, service,filename);
	exit(0);
}
/*------------------------------------------------------------------------
* TCPdaytime - invoke Daytime on specified host and print results
*------------------------------------------------------------------------
*/
int TCPtime(const char *host, const char *service,const char *filename)
{

	int s1,n; /* socket, read count */
	char buffer[1001];
	FILE *fileCopy;

	/*Client connecting to the Daytime server 1 at : */
	s1 = connectTCP(host, service);
	//bzero(buffer,1001);
	//fgets(buffer,1000,stdin);
	n = write(s1,filename,sizeof(filename));
	if((n=recv(s1, buffer, 1000,0)) > 0){
		if(buffer[0]=='1'){
		printf("Unable to copy. File does not exist in server\n");
		}
		else{
			fileCopy= fopen(filename, "w+");
			buffer[n]='\0';
			fprintf(fileCopy,"%s\n",buffer);
			bzero(buffer,n);
		}
	}
	return 0;
}

