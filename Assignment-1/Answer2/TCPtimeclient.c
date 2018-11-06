/* TCPdaytime.c - TCPdaytime, main */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <netinet/in.h>
#define BUFSIZE 64
#define UNIXEPOCH 2208988800 /* UNIX epoch, in UCT secs */
extern int errno;
int TCPtime(const char *host1,const char *host2,const char *service);
int errexit(const char *format, ...);
int connectTCP(const char *host, const char *service);
/*------------------------------------------------------------------------
* main - TCP client for DAYTIME service
*------------------------------------------------------------------------
*/
int
main(int argc, char *argv[])
{
	char *host1 = "localhost";
	char *host2 = "128.138.140.44";
	char *service = "13"; /* default service port */
	switch (argc) {
		case 1:
			host1 = "localhost";
			host2 = "localhost";
			break;
		case 4:
			service = argv[3];
			/* FALL THROUGH */
		case 3:
			host2 = argv[2];
		case 2:
			host1 = argv[1];
			break;
		default:
			fprintf(stderr, "usage: TCPdaytime [host1 [host2 [port]]]\n");
			exit(1);
	}	
	TCPtime(host1,host2, service);
	exit(0);
}
/*------------------------------------------------------------------------
* TCPdaytime - invoke Daytime on specified host and print results
*------------------------------------------------------------------------
*/
int TCPtime(const char *host1,const char *host2, const char *service)
{
	time_t now1; /* 32 int value for time */
	time_t now2;
	double diff;
	int s1,s2,n1, n2; /* socket, read count */

	/*Client connecting to the Daytime server 1 at : */
	s1 = connectTCP(host1, service);
	n1 = read(s1, (char *)&now1, sizeof(now1));
	if (n1 < 0) 
		{errexit("read failed: %s\n", strerror(errno));}
	now1 = ntohl((u_long)now1); /* put in host byte order */
	printf("time from the first server:%s", ctime(&now1));

	s2 = connectTCP(host2, service);
	n2 = read(s2, (char *)&now2, sizeof(now2));
	if (n2 < 0) 
		{errexit("read failed: %s\n", strerror(errno));}
	now2 = ntohl((u_long)now2); /* put in host byte order */
	printf("time from the second server:%s", ctime(&now2));
	diff = difftime(now2,now1);
	//now2 -= UNIXEPOCH; /* convert UCT to UNIX epoch */
	//now1 -= UNIXEPOCH; /* convert UCT to UNIX epoch */
	printf("Time Difference:%f\n", diff);
	return 0;
}

