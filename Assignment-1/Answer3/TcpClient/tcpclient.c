#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int valid = 1;
    int sd, portno, n,integerNumber;
    float num;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
/*Creating a socket*/
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) 
        error("ERROR opening socket");

/*Saving host ip to server*/
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

/*Trying to connect to the specified host on the specified port number*/
    if (connect(sd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
/*takes as input the number for finding square root */
    while(valid)
     {
      printf("Please enter the number: ");
      bzero(buffer,256);
      fgets(buffer,255,stdin);
      valid = 0;
      unsigned int i=0;
      for (i = 0;i < strlen(buffer)-1; ++i)
       {
       if (!isdigit(buffer[i]))
         {
           valid = 1;
	   printf("Number entered is not valid.");
           break;
         }
       }
      }
/*Changes the char* input to integer */
    sscanf(buffer,"%d", &integerNumber);
        
    n = write(sd,buffer,strlen(buffer));
/* Error handling */
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    sscanf(buffer,"%f", &num);
    printf("Square root of %d is %f\n",integerNumber,num);
    close(sd);
    return 0;
}
