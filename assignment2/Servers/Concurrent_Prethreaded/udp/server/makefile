LDLIBS = -lpthread
all: output

output: errexit.c passivesock.c passiveUDP.c udpserver.c
	gcc -Wall -Wextra errexit.c passivesock.c passiveUDP.c udpserver.c -o output $(LDLIBS)

clean: 
	rm output
