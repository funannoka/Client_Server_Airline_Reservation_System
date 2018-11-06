This is the README file for Concurrent Pre-forked Multiforking server program TCP and UDP protocol.

CONTENTS OF THE PACKAGE:

tcpserver.txt
errexit.c
passivesock.c
passiveTCP.c
makefile

SYSTEM REQUIREMENTS:

LINUX OPERATING SYSTEM
GNU COMPILER

CODE COMPILATION:

>$ make

OUTPUT COMMAND:
>$ output

DESCRIPTION:

This program creates a file server at a given port using Concurrent Pre-forked Multiprocessing server. When a client requests a particular file which is in the server, the file is sent to the client using the port which is assigned and the fork details are printed in the CLI.

The program has 4 modules:

tcpserver.c : Main server program which uses UDP or TCP protocol to create a server at a specific port.
passivesock.c: Contains socket allocation details. It allocates either a datagram or a stream socket, binds the socket to the well-known port for the service and returns the socket descriptor to the caller.
passiveTCP.c: Allocates a socket and bind it to a well-known port for a service.
errexit.c: Prints the error occurred and exits.

EXAMPLE OUTPUT:

>$ ./output 4343 5

fork 4 starting
fork 3 starting
fork 2 starting
fork 1 starting
fork 0 starting

**When the file is not present in the server the following is displayed in client program**

Unable to copy. File does not exist in the server.
