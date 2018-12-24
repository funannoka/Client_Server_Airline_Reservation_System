LDLIBS= -lpthread -lsqlite3
all: output

output: errexit.c passivesock.c passiveTCP.c tcpserver.c airlinemain.c customer_services.c
	gcc -Wall -Wextra errexit.c passivesock.c passiveTCP.c tcpserver.c airlinemain.c customer_services.c -o output $(LDLIBS)

clean: 
	rm output
