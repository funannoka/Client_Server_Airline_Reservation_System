all: output

output: connectsock.c connectTCP.c errexit.c passivesock.c passiveTCP.c TCPtimeclient.c
	gcc -Wall -Wextra connectsock.c connectTCP.c errexit.c passivesock.c passiveTCP.c TCPtimeclient.c -o output

clean: 
	rm output
