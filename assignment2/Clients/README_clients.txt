This is the README file for clients used to run the file server programs.

CONTENTS OF THE PACKAGE:

Folder name : tcp

tcpclient.c
errexit.c
passivesock.c
passiveTCP.c
connectTCP
connectsock.c
makefile

Folder name : udp

udpclient.c
errexit.c
passiveUDP.c
connectsock.c
connectUDP.c
makefile

SYSTEM REQUIREMENTS:

LINUX OPERATING SYSTEM
GNU COMPILER

CODE COMPILATION:

>$ make

OUTPUT COMMAND:
>$ output

DESCRIPTION:

These client programs uses TCP and UDP protocol to fetch a file from a server at a specified port.

The programs have 5 modules:

tcpclient.c/udpclient.c : Main client programs which is used to copy a file from the server at a particular port.
passivesock.c: Contains socket allocation details. It allocates either a datagram or a stream socket, binds the socket to the well-known port for the service and returns the socket descriptor to the caller.
passiveTCP.c/ passiveUDP.c: Allocates a socket and bind it to a well-known port for a service. 
errexit.c: Prints the error occurred and exits.
connectTCP.c/connectUDP.c: used to allocate a socket and fill in the basic information.


EXAMPLE OUTPUT:

>$ ./output 127.0.0.1 5500

**file gets copied**

**if file is not present in the server**

Unable to copy. File does not exist in the server.

