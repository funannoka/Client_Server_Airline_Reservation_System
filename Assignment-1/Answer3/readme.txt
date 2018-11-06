This is the README file for a TCP client and server program

CONTENTS OF THE PACKAGE
——————————————
TCP_client.c
TCP_server.c

SYSTEM REQUIREMENTS:
————————————-
LINUX Operating System
GNU Compiler

CODE COMPILATION:
——————————
Terminal 1:

$> cd TcpServer
$> make

Terminal 2:

$> cd TcpClient
$> make


Detailed description:

This contains 2 programs, a TCP client and a TCP server program. It is programmed to have the TCP client establish a connection with the TCP server, after which it passes an integer to the server as a command line argument. The server receives the message and then returns the square root of the integer back to the client, the client then prints out the returned computation by the server as well as the original number. Error reports are made if arguments other than integers are passed to the TCP server. 

EXAMPLE OUTPUT
—————————
Terminal 1: 

$> ./output 5500
Waiting for the request….

Terminal 2:

$> ./output 127.0.0.1 5500
Please enter the number: 4
Square root of 4 is 2.00000
