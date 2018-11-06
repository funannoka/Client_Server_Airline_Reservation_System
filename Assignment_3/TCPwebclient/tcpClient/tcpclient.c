/* TCPdaytime.c - TCPdaytime, main */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <mach/mach_time.h>
#include <time.h>
#include <netinet/in.h>
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif


#define PAGE "/"
#define PORT 80
#define USERAGENT "HTMLGET 1.0"


extern int errno;
int TCPtime(const char *host,const char *service,const char *filename);
int errexit(const char *format, ...);
int connectTCP(const char *host, const char *service);
char *build_get_query(char *host, char *page);
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
	struct timespec tstart={0,0}, tend={0,0};
    struct timespec ts;
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
    
//	clock_gettime(CLOCK_REALTIME, &tstart);
	int s1,n; /* socket, read count */
	char buffer[1001];
	FILE *fileCopy;
	char *get;
	/*Client connecting to the Daytime server 1 at : */
	s1 = connectTCP(host, service);
	get = build_get_query(host, filename);
	//fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);

	//Send the query to the server
	int sent = 0;
	while(sent < strlen(get))
	{
		n = send(s1, get+sent, strlen(get)-sent, 0);
		if(n == -1){
		perror("Can't send query");
		exit(1);
	}
	sent += n;
	}
	char tok[1001];
			bzero(tok,1001);
	if((n=recv(s1, buffer, 1000,0)) > 0){
		strncpy(tok,&buffer[9],3);
		tok[3]='\0';
		if(strcmp(tok,"200")==0){		
			buffer[n]='\0';
			strncpy(tok,&buffer[101],strlen(buffer)-20-101);
			fileCopy= fopen(filename, "w+");
			fprintf(fileCopy,"%s\n",tok);
			printf("No of bytes read: %d\n",strlen(tok)+1);
			bzero(buffer,n);
            clock_get_time(cclock, &mts);
			printf("File transfer Successfull!!!\nIt took about %.9f seconds\n",
			   ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
			   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
		}
		else if(strcmp(tok,"404")==0){
			printf("404 File not found\n");
            clock_get_time(cclock, &mts);
			printf("This conversation took about %.9f seconds\n",
			((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
			((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
		}
	}
		return 0;
}

char *build_get_query(char *host, char *page)
{
  char *query;
  char *getpage = page;
  char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
  if(getpage[0] == '/'){
    getpage = getpage + 1;
    //fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
  }
  // -5 is to consider the %s %s %s in tpl and the ending \0
  query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);
  sprintf(query, tpl, getpage, host, USERAGENT);
  return query;
}
