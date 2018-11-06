This is the README file for a TCP time client program

CONTENTS OF THE PACKAGE

TCPtimeclient.c
connectsock.c
connectTCP.c
passiveTCP.c
passivesock.c
errexit.c

SYSTEM REQUIREMENTS:

LINUX Operating System
GNU Compiler

CODE COMPILATION:

>$ make

This creates an output executable file called output.

Detailed description:
This is a TCP client program that connects with 2 time servers (local host or any other NIST global servers), and requests for the current date and time, after which the client returns the difference between the date and time returned by both servers.

This application has 6 modules:

TCPtimeclient.c: TCP client for time service that connects to two different servers and requests for time. It reads the two different times and prints the difference between the two time.
connectsock.c: Allocation and connecting a socket using TCP or UDP 
connectTCP.c: Connects the application to a specified TCP service on a specified host
errexit.c: Prints the error occurred and exits
passiveTCP.c - Creates a passive socket for use in a TCP server
passivesock.c - Allocates & binds a server socket using TCP or UDP

EXAMPLE OUTPUT


>$ ./output
time from the first server:Sun Aug 20 04:45:19 1995
time from the second server:Sun Aug 20 04:45:19 1995
Time Difference:0.000000
