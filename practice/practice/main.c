//
//  main.c
//  practice
//
//  Created by Ifunanya Nnoka on 12/16/16.
//  Copyright © 2016 Ifunanya Nnoka. All rights reserved.
//

#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include<errno.h>
extern int errno;
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#define __USE_BSD 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <sqlite3.h>
#include <semaphore.h>
#include <sys/mman.h>

volatile int stop = 0;

void handler (int NotUsed)
{
    stop = 1;
}

void event_loop (int sock)
{
    signal (SIGINT, handler);
    
    while (1) {
        if (stop) {
            printf ("do cleanup\n");
            return;
        }
        char buf [1];
        int rc = recv (sock, buf, 1, 0);
        if (rc == -1 && errno == EINTR)
            continue;
        printf ("perform an action\n");
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
