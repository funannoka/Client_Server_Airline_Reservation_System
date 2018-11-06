/* UDPtime.c - main */
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#define STRLEN 1001
#define MESSAGE "what day and what time is it?\n"
typedef unsigned long u_long;
extern int errno;
int connectUDP(const char *host, const char *service);
int errexit(const char *format, ...);
/*------------------------------------------------------------------------
* main - UDP client for TIME service that prints the resulting time
*------------------------------------------------------------------------
*/
int
main(int argc, char *argv[])
{
	FILE *fileCopy;
	char *filename = "";
	char buffer[1001];
	char *server_ip = "localhost"; /* host to use if none supplied */
	char *service_name = "5500"; /* default service name */
	//char *now; /* 32-bit integer to hold time */
	int s, n; /* socket, read count */
	switch (argc) {
		case 4: filename = argv[3];
			/*FALL THROUGH*/
		case 3:
			service_name = argv[2];
			/* continue*/
		case 2:
			server_ip = argv[1];
			break;
		default:
			fprintf(stderr, "usage: UDPtime [host [port]]\n");
			exit(1);
		}
	/* Read the day time from server */
	printf("%s\n",service_name);
	s = connectUDP(server_ip, service_name);
	/* Write to the server, asking for the time. Daytime server only responds if its gets a request from the client*/
	(void) write(s, filename, strlen(filename));

	//bzero(s,1001);
	if((n=read(s, (char *)&buffer, 1001)) > 0){
		if(buffer[0]=='1'){
			printf("Unable to copy. File does not exist in server\n");
		}
		else{
			fileCopy= fopen(filename, "w+");
			buffer[n]='\0';
			fprintf(fileCopy,"%s\n",buffer);
			bzero(buffer,n);
			fclose(fileCopy);
		}}
	else{
		errexit("socket read failed: %s \n", strerror(errno));		
	}
	exit(0);
}
