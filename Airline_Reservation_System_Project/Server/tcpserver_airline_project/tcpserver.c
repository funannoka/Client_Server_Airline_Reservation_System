
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
#include "database.h"
#include <semaphore.h>
#include <sys/mman.h>
#include "generateDigest.h"
#include "encrypt.h"
typedef void sigfunc (int);
#undef socklen_t
//#include "unp.h"
#define MAXN 16384  /* max # bytes client can request */
#define LISTENQ 50
#define MAXLINE 2048
#define QLEN 5 /* maximum connection queue length */
#define BUFSIZE 4096
int errexit(const char *format, ...);
int static nchildren;
static pid_t *pids;
sem_t sock_lock_db_call_result;
void AirlineMain(void* sock);
void Customer_funct(void* sock);
int login_(char* credentials,char* User);
int verify(char* uname,char* password);
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);
extern int errno;
void customer(void* sock);
void flight(void* sock);
char *Succ_Login = "\n\nLogged in Successfully!!!!!\n";
char *Failure_Login = "\n****ERROR****Incorrect Username or Password\n Please Enter again <Username> <Password>\n";void system_admin(void *ssock);
void reaper(int);
void *Server(void *vargp);
int errexit(const char *format, ...);
int passiveTCP(const char *service, int qlen);
extern int callback_view_users(void *data, int argc, char **argv, char **azColName);
char db_call_result[1500];








/*---------------Function declarations------------------*/
int pthread_mutex_lock(pthread_mutex_t *mptr);
int pthread_mutex_unlock(pthread_mutex_t *mptr);
int pthread_create(pthread_t *tid, const pthread_attr_t *attr, void *(*func) (void *), void *arg);


/*-------------------Header that defines a thread structure that maintains info about each thread------------- */

typedef struct
{
 pthread_t thread_tid; /* thread ID */
 long thread_count; /* # connections handled */
 }thread;
 
 thread *tptr; /* array of Thread structures; calloc'ed */
 int listenfd, nthreads;
 socklen_t addrlen;
 pthread_mutex_t mLock;
 
/*-------------------------main---------------------------*/
 
 
 pthread_mutex_t mLock = PTHREAD_MUTEX_INITIALIZER;
int main(int argc, char **argv)
{
	int i;
	void sig_int(int), thread_make(int);

	rc = sqlite3_open("airline.db", &db);
	if (rc != SQLITE_OK) {
		printf("ERROR opening SQLite DB 'airline': %s\n", sqlite3_errmsg(db));
	}
   int o = sqlite3_threadsafe();
    printf("sqlite_threadsafe sys_admin: %d\n",o);
	//char* name = "System_Admin";
   // char* password = "a41acc7effe601de1dc2099a4e2fdd7c";
  //  char *uid = "000";
  //  char *airline = "N/A";

	//asprintf(&query, "create table Flight_Users"\
    "(name text PRIMARY KEY NOT NULL,password text NOT NULL,"\
    "airline text NOT NULL,UID int NOT NULL);");
	//execute_command_db();
  //  asprintf(&query, "insert into Flight_Users (name,password,airline,uid)"\
    "values ('%s','%s','%s',%s);",name,password,airline,uid);
  //  execute_command_db();
	if (argc == 2)
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 4)
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);

	else
		errexit("usage: serv07 [ <host> ] <port#> <#threads>");
	nthreads = 50;
	tptr = calloc(nthreads, sizeof(thread));
	for (i = 0; i < nthreads; i++)
		thread_make(i); /* only main thread returns */
	signal(SIGINT, sig_int);
	for ( ; ; )
		pause(); /* everything done by threads */

}

/*-------------------------thread make and thread main functions---------------*/

 
 void thread_make(int i)
 {
 void *thread_main(void *);
 pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *) i);
 return; /* main thread returns */
  }
  
  
 void * thread_main(void *arg)
 {
 int connfd;
 void web_child(int);
 socklen_t clilen;
 struct sockaddr *cliaddr;
 cliaddr = malloc(addrlen);
 //printf("thread %d starting\n", (int) arg);
 for ( ; ; ) 
 {
 clilen = addrlen;
 pthread_mutex_lock(&mLock);
 connfd = accept(listenfd, cliaddr, &clilen);
 pthread_mutex_unlock(&mLock);
 tptr[(int) arg].thread_count++;
 web_child(connfd); 					/* process request */
 close(connfd);
 }
 
 }

 /*-----------------------------------TCP listen---------------------*/
 
 
 int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
 {
   int listenfd, n;
   const int on = 1;
   struct addrinfo hints, *res, *ressave;
   bzero(&hints, sizeof (struct addrinfo)) ;
   hints.ai_flags = AI_PASSIVE;
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   
   if ((n = getaddrinfo (host, serv, &hints, &res)) != 0)
   errexit("tcp_listen error for %s, %s: %s",host, serv, gai_strerror(n)) ;
   ressave = res;
  do
  {
  listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (listenfd < 0)
  continue; /* error, try next one */
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on) ) ;
  if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
  break; /* success */
  close (listenfd); /* bind error, close and try next one */
 }
 while ( (res = res->ai_next) != NULL);
 
   if (res == NULL) /* errno from final socket () or bind () */
      errexit ("tcp_listen error for %s, %s", host, serv);
       listen (listenfd, LISTENQ);
    if (addrlenp)
      *addrlenp = res->ai_addrlen; /* return size of protocol address */
      freeaddrinfo (ressave);
     return (listenfd);

 }
 
 
 
/*---------Signal Handler for SIG_INT------------------------------------*/

 void sig_int(int signo)
 {
 int i;
 void pr_cpu_time(void);
 /* terminate all children */
 
 for (i = 0; i < nchildren; i++)
 kill(pids[i], SIGTERM);
 while (wait(NULL) > 0) /* wait for all children */
 ;
 
 if(errno != ECHILD)
 errexit("wait error");
 
 pr_cpu_time();
 exit(0);
 }
 
 /*------------------wrapper function for---- Pthread_mutex_lock------------*/
 
 void Pthread_mutex_lock(pthread_mutex_t *mptr)
{
int n;
if((n=pthread_mutex_lock(mptr))==0)
return;
errno=n;
errexit("pthread_mutex_lock error");
}


/*------------------wrapper function for---- Pthread_mutex_unlock------------*/
 
 void Pthread_mutex_unlock(pthread_mutex_t *mptr)
{
int n;
if((n=pthread_mutex_unlock(mptr))==0)
return;
errno=n;
errexit("pthread_mutex_unlock error");
}

#include    <sys/resource.h>
#ifndef HAVE_GETRUSAGE_PROTO
int     getrusage(int, struct rusage *);
#endif

void pr_cpu_time(void)
 {
      double user, sys;
     struct rusage myusage, childusage;

     if (getrusage(RUSAGE_SELF, &myusage) < 0)
         errexit("getrusage error");
     if (getrusage(RUSAGE_CHILDREN, &childusage) < 0)
         errexit("getrusage error");

     user = (double) myusage.ru_utime.tv_sec +
         myusage.ru_utime.tv_usec / 1000000.0;
     user += (double) childusage.ru_utime.tv_sec +
         childusage.ru_utime.tv_usec / 1000000.0;
     sys = (double) myusage.ru_stime.tv_sec +
         myusage.ru_stime.tv_usec / 1000000.0;
     sys += (double) childusage.ru_stime.tv_sec +
         childusage.ru_stime.tv_usec / 1000000.0;

   //  printf("\nuser time = %g, sys time = %g\n", user, sys);
 }

void web_child(int ssock)
 {

	struct sockaddr_in fsin; /* the address of a client */
	socklen_t alen; /* length of client's address */
	char client_buffer[2701];
	//char buff[1001];
	char* user_sa="1";
	char* user_fa="2";
	char* user_cust="3";
    char* user_exit = "4";
    int true = 1;
     char *decrypt;
     char *encrypt;
	char *Welcome_screen = "\n \nHi! Welcome to Airline reservation system. Please enter the user functionality you would like to access today.\n1. System Admin 2. Flight services 3. Customer 4. Exit\n";
     int n,n1;
	alen = sizeof(fsin);
	printf("thread executing\n");
	bzero(client_buffer,sizeof(client_buffer));
	/*Reading value from the client*/

    while(true){
	n = read(ssock,client_buffer,sizeof(client_buffer));
	if (n < 0) {
		errexit("ERROR reading from socket %s\n", strerror(errno));
		}
	bzero(client_buffer,sizeof(client_buffer));
	printf("writing to the client\n");
	strcpy(client_buffer,Welcome_screen);
    encrypt = Encrypt(client_buffer);
    n1=write(ssock,encrypt,strlen(encrypt));
	if (n1 <= 0){
		errexit("Server:ERROR writing to socket\n");
		}
	printf("reading from the client\n");
	bzero(client_buffer,sizeof(client_buffer));
	if((n = read(ssock,client_buffer,sizeof(client_buffer))>0)){
        decrypt=Decrypt(client_buffer);
        bzero(client_buffer,sizeof(client_buffer));
        strcpy(client_buffer,decrypt);
        if(strcmp(client_buffer,user_sa)==0){
            system_admin((void*)ssock);
		}
		if(strcmp(client_buffer,user_fa)==0){
        AirlineMain((void*)ssock);
		}
		if(strcmp(client_buffer,user_cust)==0){
        Customer_funct((void*)ssock);
        }
        if(strcmp(client_buffer,user_exit)==0){
        bzero(client_buffer,sizeof(client_buffer));
        printf("writing to the clilent\n");
        strcpy(client_buffer,"Have a Wonderfull day!!!!\n");
        encrypt = Encrypt(client_buffer);
        n1=write(ssock,encrypt,strlen(encrypt));
        if (n1 <= 0){
            errexit("Server:ERROR writing to socket\n");
        }
            true = 0;
            (void) close(ssock);

        }
	}
	else{ 
		printf("Error readng from the socket");
        (void) close(ssock);
		}
     }
	//(void) close(ssock);
	//	sem_post(&sock_lock);
}



void system_admin(void* sock){
    char* sql;
    sql = "SELECT name from Flight_Users;";
    char* errmsg;
    char *zErrMsg = 0;
    char *decrypt;
    char *encrypt;
    int rc;
    rc = sqlite3_open("airline.db", &db);
    if( rc ){
        fprintf(stderr, "Database failed to open: %s\n", sqlite3_errmsg(db));
    }
    else
    fprintf(stderr, "Database opened successfully\n");
    sem_init(&sock_lock_db_call_result,0,1);
    //sem_t * sem_open(const char *, int, ...);
    int ssock= (int) sock;
    int n1;
    char  *paswdd_hash;
    char airline_name[50];
    char client_buffer[2701];
    char *Welcome_screen_sa = "\nWelcome System Admin. Please login. Enter <Username> <Password>\n\n";
    char *Sys_admin_options= "\n\nWhat would you like to do next\n\n1. Add User 2. Modify Password for any user 3.View Users 4.Delete a User 5.Exit\n";
    char *add_user= "\n\nPlease enter user details to add a user: <username> <password> <airline> <UID>:\n\n";
    char *modify_user= "\n\nPlease enter new password: <Username> <password>:\n\n";
    bzero(client_buffer,sizeof(client_buffer));
    printf("writing to the clilent\n");
    strcpy(client_buffer,Welcome_screen_sa);
    encrypt = Encrypt(client_buffer);
    n1=write(ssock,encrypt,strlen(encrypt));
    if (n1 <= 0){
        errexit("Server:ERROR writing to socket\n");
    }
    while(1){
        bzero(client_buffer,sizeof(client_buffer));
        /*Reading value from the client*/
        
        n1 = read(ssock,client_buffer,sizeof(client_buffer));
        decrypt=Decrypt(client_buffer);
        bzero(client_buffer,sizeof(client_buffer));
        strcpy(client_buffer,decrypt);
        if (n1 < 0) {
            errexit("ERROR reading from socket %s\n", strerror(errno));
        }
        if(login_(client_buffer,"System_Admin")==1){
            bzero(client_buffer,sizeof(client_buffer));
            printf("writing to the client\n");
            strcpy(client_buffer,Succ_Login);
            encrypt = Encrypt(client_buffer);
            n1=write(ssock,encrypt,strlen(encrypt));
            pass_match = 0;
            break;
        }
        else {
            bzero(client_buffer,sizeof(client_buffer));
            printf("writing to the client\n");
            strcpy(client_buffer,Failure_Login);
            encrypt = Encrypt(client_buffer);
            n1=write(ssock,encrypt,strlen(encrypt));
        }
        
        if (n1 <= 0){
            errexit("Server:ERROR writing to socket\n");
        }
    }
    while(1){
        bzero(client_buffer,sizeof(client_buffer));
        printf("writing to the client\n");
        strcat(client_buffer,Sys_admin_options);
        encrypt = Encrypt(client_buffer);
        n1=write(ssock,encrypt,strlen(encrypt));
        if (n1 <= 0){
            errexit("Server:ERROR writing to socket\n");
        }
        bzero(client_buffer,sizeof(client_buffer));
        
        n1 = read(ssock,client_buffer,sizeof(client_buffer));
        decrypt=Decrypt(client_buffer);
        bzero(client_buffer,sizeof(client_buffer));
        strcpy(client_buffer,decrypt);
        if (n1 < 0) {
            errexit("ERROR reading from socket %s\n", strerror(errno));
        }
        if(strcmp(client_buffer,"1")==0){
            bzero(client_buffer,sizeof(client_buffer));
            printf("writing to the clilent\n");
            strcpy(client_buffer,add_user);
            encrypt = Encrypt(client_buffer);
            n1=write(ssock,encrypt,strlen(encrypt));
            if (n1 <= 0){
                errexit("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer,sizeof(client_buffer));
            n1 = read(ssock,client_buffer,sizeof(client_buffer));
            decrypt=Decrypt(client_buffer);
            bzero(client_buffer,sizeof(client_buffer));
            strcpy(client_buffer,decrypt);
            if (n1 < 0) {
                errexit("ERROR reading from socket %s\n", strerror(errno));
            }
            char* name=NULL;
            char* password=NULL;
            char* uid = NULL;
            char* airline = NULL;
            name = strtok(client_buffer, " ");
            int i=0;
            while(i<1)
            {
                password = strtok(NULL, " ");
                airline = strtok(NULL, " ");
                uid = strtok(NULL, " ");
                i++;
            }
            paswdd_hash = gen_hash_md5(password,strlen(password));
            asprintf(&query, "insert into Flight_Users(name,password,airline,uid) values('%s','%s','%s',%s);",name,paswdd_hash,airline,uid);
            rc = sqlite3_exec(db, query, 0,0, &zErrMsg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            free(query);
            //records airline and uid to uid table if it doesnt already exist
            asprintf(&query, "insert into UID_STORE (UID,AIRLINE) values (%s,'%s');",uid,airline);
            rc = sqlite3_exec(db, query, 0,0, &zErrMsg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            free(query);
           // bzero(client_buffer,1001);
            free(paswdd_hash);
        }
        if(strcmp(client_buffer,"2")==0){
            bzero(client_buffer,sizeof(client_buffer));
            sem_wait(&sock_lock_db_call_result);
            printf("writing to the client\n");
            strcpy(client_buffer,modify_user);
            encrypt = Encrypt(client_buffer);
            n1=write(ssock,encrypt,strlen(encrypt));
            if (n1 <= 0){
                errexit("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer,sizeof(client_buffer));
            n1 = read(ssock,client_buffer,sizeof(client_buffer));
            decrypt=Decrypt(client_buffer);
            bzero(client_buffer,sizeof(client_buffer));
            strcpy(client_buffer,decrypt);
            if (n1 < 0) {
                errexit("ERROR reading from socket %s\n", strerror(errno));
            }
            
            char* name=NULL;
            char* password=NULL;
            name = strtok(client_buffer, " ");
            int i=0;
            while(i<1)
            {
                password = strtok(NULL, " ");
                i++;
            }
            paswdd_hash = gen_hash_md5(password,strlen(password));
            asprintf(&query, "Update Flight_Users set password = '%s' where name = '%s';",paswdd_hash,name);
            execute_command_db();
            sem_post(&sock_lock_db_call_result);
            free(paswdd_hash);
        }
        if(strcmp(client_buffer,"3")==0){
            bzero(client_buffer,sizeof(client_buffer));
            //sem_wait(&sock_lock_db_call_result);
            rc = sqlite3_exec(db, sql, callback_view_users, NULL, &errmsg);
            if (rc != SQLITE_OK) {
                sqlite3_free(errmsg);
            }
            bzero(client_buffer,sizeof(client_buffer));
            strcpy(client_buffer,"\n\nUsers of this system are: \n");
            strcat(client_buffer,db_call_result);
            
          //  sqlite3_finalize(stmt);
            printf("writing to the client\n");
            encrypt = Encrypt(client_buffer);
            n1=write(ssock,encrypt,strlen(encrypt));
            strcpy(db_call_result,"");
            //sem_post(&sock_lock_db_call_result);
            if (n1 <= 0){
                errexit("Server:ERROR writing to socket\n");
            }
        }
        if(strcmp(client_buffer,"4")==0){
            bzero(client_buffer,sizeof(client_buffer));
            printf("writing to the client\n");
            
            strcpy(client_buffer,"\n\nPlease enter the name of user to be deleted:<User Name>\n\n");
            encrypt = Encrypt(client_buffer);
            n1=write(ssock,encrypt,strlen(encrypt));
            if (n1 <= 0){
                errexit("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer,sizeof(client_buffer));
            n1 = read(ssock,client_buffer,sizeof(client_buffer));
            decrypt=Decrypt(client_buffer);
            bzero(client_buffer,sizeof(client_buffer));
            strcpy(client_buffer,decrypt);
            sem_wait(&sock_lock_db_call_result);
            if (n1 < 0) {
                errexit("ERROR reading from socket %s\n", strerror(errno));
            }
            strcpy(airline_name, client_buffer);
            asprintf(&query, "Delete from Flight_Users where name = '%s';",client_buffer);
            execute_command_db();
            printf("writing to the client\n");
            bzero(client_buffer,sizeof(client_buffer));
            strcpy(client_buffer,db_call_result);
            encrypt = Encrypt(client_buffer);
            n1=write(ssock,encrypt,strlen(encrypt));
            strcpy(db_call_result,"");
            if (n1 <= 0){
                errexit("Server:ERROR writing to socket\n");
            }
            sem_post(&sock_lock_db_call_result);
        }
        if(strcmp(client_buffer,"5")==0){
            bzero(client_buffer,sizeof(client_buffer));
            printf("writing to the client\n");
            strcpy(client_buffer,"Goodbye!");
            encrypt = Encrypt(client_buffer);
            n1=write(ssock,encrypt,strlen(encrypt));
            if (n1 <= 0){
                errexit("Server:ERROR writing to socket\n");
            }
            return;
        }
    }
}

int login_(char* credentials,char* User){
	char* token=NULL;
	char* password=NULL;
	token = strtok(credentials, " ");
	int i=0;
	while( token != NULL && i<1) 
	{
		password = strtok(NULL, " ");
		i++;
	}
	if(strcmp(User,"System_Admin ")==0){
		if(!(strcmp(token,"System_Admin")))
			return 0;
	}
	if(password!=NULL){
		return verify(token,password);}
	else return 0;
}


int verify(char* uname,char* password){
	char *errmsg;
	char *sql;
	char buffer[256];
    char *paswd_hash;
    int rc;
    
    strcpy(buffer, "SELECT password from Flight_Users where name='");
    strcat(buffer, uname);
    strcat(buffer, "';");
	sql = buffer;                           /* 1 */
    paswd_hash = gen_hash_md5(password,strlen(password));
	rc = sqlite3_exec(db, sql, callback, (void*) paswd_hash, &errmsg);
	
    if (rc != SQLITE_OK) {
		printf("OK");	
		fprintf(stderr, "SQL error: %s\n", errmsg);
		sqlite3_free(errmsg);
	}
	//sqlite3_finalize(stmt);
	return pass_match;
}
