#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sd, newsd, portno;
     int num = 0;
     float answer=0;
     socklen_t clen;
     char fromclient[256];
     struct sockaddr_in server_addr, client_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

/*Creating a socket for listening*/
     sd = socket(AF_INET, SOCK_STREAM, 0);
     if (sd < 0) 
        error("ERROR opening socket");
     bzero((char *) &server_addr, sizeof(server_addr));
     portno = atoi(argv[1]);

/*Setting parameters for connection*/
     server_addr.sin_family = AF_INET;
     server_addr.sin_addr.s_addr = INADDR_ANY;
     server_addr.sin_port = htons(portno);

/*Binding port number to the socket to provide service for any client connecting to this port*/
     if (bind(sd, (struct sockaddr *) &server_addr,
              sizeof(server_addr)) < 0) 
              error("ERROR on binding");

/*Waiting for client request using listen call */
     listen(sd,5);
     printf("Waiting for request...\n");
     clen = sizeof(client_addr);

/*Accpeting a request from client. Program blocks here to receive a request from any client*/
     newsd = accept(sd, 
                 (struct sockaddr *) &client_addr, 
                 &clen);
     if (newsd < 0) 
          error("ERROR on accept");
     bzero(fromclient,256);
/*Reading value from the client*/
     n = read(newsd,fromclient,255);
     if (n < 0) error("ERROR reading from socket");
     else 
	{

/*Processing value from client to get Square root of the number*/
	sscanf(fromclient,"%d", &num);
	answer = sqrt(num);
	}
     sprintf(fromclient,"%lf", answer);

/*Writing result back to the client*/
     n = write(newsd,fromclient,sizeof(fromclient));
     if (n < 0) error("ERROR writing to socket");
     close(newsd);
     close(sd);
     return 0; 
}
