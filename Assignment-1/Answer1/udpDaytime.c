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
#define STRLEN 128
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
	char *daytime_server_ip = "localhost"; /* host to use if none supplied */
	char *daytime_service_name = "daytime"; /* default service name */
	//char *now; /* 32-bit integer to hold time */
	char now[STRLEN+1]; /* buffer for one line of text */
	int s, n; /* socket, read count */
	switch (argc) {
		case 1:
			daytime_server_ip = "localhost";
			break;
		case 3:
			daytime_service_name = argv[2];
			/* continue*/
		case 2:
			daytime_server_ip = argv[1];
			break;
		default:
			fprintf(stderr, "usage: UDPtime [host [port]]\n");
			exit(1);
		}
	/* Read the day time from server */
	s = connectUDP(daytime_server_ip, daytime_service_name);
	/* Write to the server, asking for the time. Daytime server only 		responds if its gets a request from the client*/
	(void) write(s, MESSAGE, strlen(MESSAGE));
	n = read(s, (char *)&now, STRLEN+1);
	/*checking if client got an empty string*/
	if (n < 0) 
		{errexit("read failed: %s\n", strerror(errno));}
	now[n]='\0';
	printf("Current time received from the time server is:\t");
	(void) fputs(now, stdout );
	exit(0);
}
