This is the README file for UDP daytime client program

CONTENTS OF THE PACKAGE

udpDaytime.c
connectsock.c
connectUDP.c
errexit.c
makefile

SYSTEM REQUIREMENTS:

LINUX Operating System
GNU Compiler

CODE COMPILATION:

>$ make

OUTPUT COMMAND:
>$ output

Detailed description:
This is a UDP client program that returns the current date and time by sending a request message to the local host server for its daytime service.

This program has 4 modules:
udpDaytime.c: UDP client for DAYTIME service that prints the resulting time
connectsock.c: Allocation and connecting a socket using TCP or UDP 
connectUDP.c: Connects the application to a specified UDP service on a specified host
errexit.c: Prints the error occurred and exits


EXAMPLE OUTPUT


>$ ./output
Current time received from the time server is:	07 OCT 2016 22:59:00 PDT

