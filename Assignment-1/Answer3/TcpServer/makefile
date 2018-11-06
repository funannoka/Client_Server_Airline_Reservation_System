LDLIBS = -lm
all: output

output: tcpserver.c
	gcc -Wall -Wextra tcpserver.c -o output $(LDLIBS)

clean: 
	rm output
