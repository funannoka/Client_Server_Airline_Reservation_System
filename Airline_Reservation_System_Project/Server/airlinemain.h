//
//  airlinemain.h
//  tcpserver_airline_project
//
//  Created by Ifunanya Nnoka on 12/1/16.
//  Copyright © 2016 Ifunanya Nnoka. All rights reserved.
//

#ifndef airlinemain_h
#define airlinemain_h
#define QLEN 5 /* maximum connection queue length */
#define BUFSIZE 4096
extern int errno;
void AirlineMain(void* sock);
int errexit(const char *format, ...);
char * extract_line(char *field,const char filename[],int no_of_bytes);
char * cmpr_airline(const char *fieeld,int no_bytes);
void create_table(void);
void create_UID_store(void);
int view_flight (const char *name);
int view_airline (const char *airline);
void add_col(void);
int view_all_flights(void);
int add_flight(const char *flight,const char *buffer);
int uid_confirm (const char *airline,const char *id);
void freebuf(char **buf);
void get_uid(const char *buffer);
void add_uid (const char *buffer);
int modify_flight(const char *flight, const char *buffer);
int delete_flight(const char *name);
char wclientMsg[] = "writing to the client\n";
char rclientMsg[] = "reading from the client\n";
int id_ok,id_,ok_;
int a_ok;
char call_buff[2701];
char call_buff2[2701];
char call_buff3[2701];
char *cb = call_buff;
char airlineNameQuery[] = "Enter Airline name: ";
char airlineIDQuery[] = "Enter Airline ID: ";
char uidAssignedUser[] = "already assigned to a different airline.\n"\
"Enter y to use a different ID, n to exit!\n";
char incorrectIDMsg[] = "Incorrect ID entered for airline .\n"\
"Enter y to use a different ID, n to exit!\n";
char flightNameQuery[] = "Enter Flight name: ";
char flightSourceQuery[] = "Enter Flight Source: ";
char flightDestinationQuery[] = "Enter Flight Destination: ";
char flightPriceQuery[] = "Enter Flight Price: ";
char totalSeatQuery[] = "Enter Total Number of Seats: ";
char openSeatsQuery[] = "Enter Number of Seats Available: ";
char departDateQuery[] = "Enter date and time of Depature: ";
char arriveDateQuery[] = "Enter date and time of arrival: ";
char flightAddSuccesMsg[] = "\nFlight added to server!\n\n";
char flightSummaryMsg[] = "Flight Summary:\n\n \n"\
"Enter y to add more flights, n to exit.";
char addflightFailMsg[] = "Undefined Error.Unable to add  to database.\n"\
"Enter y to add more flights, n to exit.";
char flightnotExistMsg[] = "\nWe're sorry that flight does not exist on our server."\
"\nEnter y to search more fields, n to exit.";
char flightnotExistMsg2[] = "\nWe're sorry that flight does not exist on our server.\n";
char uidaddedMsg[] = "Airline and UID added to server";
char airlinenotExistMsg[] = "Airline does not exist in server.\n"\
"<<:: 1. Add Airline ::>> <<:: 2. Go Back :>>";
char modifyAllMsg[] = "Enter to modify:\n<<:: 1. All ::>> <<:: 2. Source ::>> <<:: 3."\
"Destination ::>> <<:: 4. Price ::>>\n<<:: 5. No of Seats Available ::>>"\
"<<:: 6. Date of Departure ::>> <<:: 7. Date of Arrival ::>>";
char modifyflightErrMsg[] = "We're Sorry, there was an error while modifying flight."\
"\nEnter y to try again, n to exit.";
char deleteflightErrMsg[] = "We're sorry, there was an error while deleting flight.\n"\
"Enter y to try again, n to exit.";
char searchMoreMsg[] = "Enter y to search a different airline, n to exit.";
char noflightsMsg[] = "No flights available at the moment. Please try again later!";
char flightsearchResultMsg[] = "\nSearch Results: All Flights\n\n";
char flightdeleteSuccesSmSG[] = "Flight deleted successfully!\n"\
"Enter y to delete more flights, n to exit.";
sqlite3 *db2;
pthread_mutex_t addflightLock,viewflightLock;
pthread_mutex_t uidcnfrmLock, getuidLock,adduidLock;
pthread_mutex_t callbackLock,modifyflightLock;
pthread_mutex_t deleteflightLock,viewallflightLock;
pthread_mutex_t viewairlineLock;


static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        cb += sprintf(cb,"%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        if (argv[i] == NULL) {
            ok_ = 0;
        }else ok_ = 1;
    }
    cb += sprintf(cb,"\n");
    return 0;
}

static int callback2(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        sprintf(call_buff2,"%s\n",argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

static int callback3(void *data, int argc, char **argv, char **azColName)
{
    int i;
    id_= 1;
    for (i=0; i<argc; i++) {
        // sprintf("%s",argv[i] ? argv[i] : "NULL");
        if (argv[i] == NULL) {
            id_ok = 2;
        }else if(strcmp((const char*) data,argv[i])==0){
            id_ok = 1;
        }
    }
    return 0;
}
void create_table(void)
{
    char *sql;
    int rc;
    char *zErrMsg = 0;
    
    
    /*Creates SQL statement*/
    sql = "Create table data("\
    "NAME char primary key not null,"\
    "UID             int not null,"\
    "AIRLINE         text not null,"\
    "SOURCE          text not null,"\
    "DESTINATION     text not null,"\
    "PRICE           real,"\
    "TOTAL_SEATS     int not null,"\
    "OPEN_SEATS      int not null,"\
    "DATE_DEPATURE   char not null,"\
    "DATE_ARRIVAL    char not null);";
    /*Execute SQL statement*/
    rc = sqlite3_exec(db2, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        //   fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
}
void add_col(void)
{
    char *sql;
    int rc;
    char *zErrMsg = 0;
    
    sql ="ALTER TABLE data ADD COLUMN DATE_DEPATURE char;";
    /*Execute SQL statement*/
    rc = sqlite3_exec(db2, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        //   fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sql ="ALTER TABLE data ADD COLUMN DATE_ARRIVAL char;";
    /*Execute SQL statement*/
    rc = sqlite3_exec(db2, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        //   fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    
}

void create_UID_store(void)
{
    char *sql;
    int rc;
    char *zErrMsg = 0;
    
    
    /*Creates SQL statement*/
    sql = "Create table UID_STORE("\
    "UID int primary key not null,"\
    "AIRLINE         text not null);";
    
    /*Execute SQL statement*/
    rc = sqlite3_exec(db2, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        //  fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
}

void add_uid (const char *buffer)
{
    /*Creates SQL statement. Inserts information into SQL table named "uid_store"*/
    /*Execute SQL statement*/
    int rc;
    char *zErrMsg = 0;
    
    rc = sqlite3_exec(db2, buffer, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "New airline and UID added to database\n");
    }
}

int uid_confirm (const char *airline,const char *id)
{
    id_ok = 0;
    id_ = 0;
    char buff[256];
    int rc;
    char *zErrMsg = 0;
    // const char filename[] = "UIDSTORE.txt";
    sprintf(buff, "SELECT uid from uid_store where airline = '%s';",airline);
    /*Execute SQL statement*/
    rc = sqlite3_exec(db2, buff, callback3, (void*)id, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    if (id_ != 1) {
        id_ok = 2;
    }
    return id_ok;
}

void get_uid(const char *buffer)
{
    int rc;
    char *zErrMsg = 0;
    
    /*Execute SQL statement*/
    rc = sqlite3_exec(db2, buffer, callback2,0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

int add_flight(const char *flight,const char *buffer)
{
    cb = &call_buff[0];
    a_ok = 0;
    int rc;
    char buff[256];
    char *zErrMsg = 0;
    /*Creates SQL statement. Inserts information into SQL table named "data"*/
    /*Execute SQL statement*/
    rc = sqlite3_exec(db2, buffer, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        a_ok =1;
    }
    cb = &call_buff[0];
    bzero(buff, sizeof(buff));
    /*Call back added flight*/
    sprintf(buff, "SELECT * from DATA where NAME = '%s';",flight);
    /*Execute SQL statement*/
    pthread_mutex_lock(&callbackLock);
    rc = sqlite3_exec(db2, buff,callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return a_ok;
}

int view_all_flights(void)
{
    char *sql;
    int i =0;
    int rc;
    char *zErrMsg = 0;
    
    //bzero(call_buff, 2001);
    cb = &call_buff[0];
    sql = "SELECT * from DATA;";
    pthread_mutex_lock(&callbackLock);
    /*Execute SQL statement*/
    rc = sqlite3_exec(db2, sql, callback,0, &zErrMsg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        i=1;
    }
    return i;
}

int view_airline (const char *airline)
{
    char buff[256];
    int rc;
    char *sql;
    char *zErrMsg = 0;
    
    ok_ = 0;
    cb = &call_buff[0];
    bzero(buff, 256);
    /*Execute SQL statement*/
    sprintf(buff, "SELECT * from DATA where airline = '%s';",airline);
    sql = buff;
    pthread_mutex_lock(&callbackLock);
    rc = sqlite3_exec(db2, sql, callback,0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return ok_;
}

int view_flight (const char *name)
{
    ok_ = 0;
    char *zErrMsg = 0;
    int rc;
    char buffer[256];
    cb = &call_buff[0];
    bzero(buffer, sizeof(buffer));
    sprintf(buffer, "SELECT * from DATA where name = '%s';",name);
    /*Execute SQL statement*/
    pthread_mutex_lock(&callbackLock);
    rc = sqlite3_exec(db2, buffer, callback,0, &zErrMsg);
    if(rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return ok_;
}

int modify_flight(const char *flight, const char *buffer)
{
    int i = 0;
    int rc;
    char buff[256];
    char *sql;
    cb = &call_buff[0];
    char *zErrMsg = 0;
    
    /* Create UPDATE SQL statement */
    /*Execute SQL statement*/
    rc = sqlite3_exec(db2, buffer, 0,0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        i=1;
    }
    cb = &call_buff[0];
    bzero(buff, 256);
    /*Call back modified flight*/
    sprintf(buff,"SELECT * from DATA where NAME ='%s';",flight);
    sql = buff;
    /*Execute SQL statement*/
    pthread_mutex_lock(&callbackLock);
    rc = sqlite3_exec(db2, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return i;
    
}


int delete_flight(const char *name)
{
    int n = 0;
    char *sql;
    char buff[256];
    int rc;
    char *zErrMsg = 0;
    
    bzero(buff, 256);
    /* Create DELETE SQL statement */
    sprintf(buff,"DELETE from DATA where NAME = '%s';",name);
    sql = buff;
    /*Execute SQL statement*/
    rc = sqlite3_exec(db2, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        n = 1;
    }
    return n;
}
char * extract_line(char *field,const char filename[],int no_of_bytes)
{
    // static const char filename[] = "view_flight.txt";
    char line_temp[50];
    char *line =  (char *)malloc(sizeof(char) * 50);
    FILE *pfile = fopen(filename,"r");
    if (pfile != NULL)
    {
        while (fgets(line_temp, sizeof(line_temp), pfile)!= NULL) {
            
            if (strncmp(line_temp, field,no_of_bytes)==0)
            {
                strcpy(line, line_temp);
            }
        }
    }
    fclose(pfile);
    return line;
}
char * cmpr_airline(const char *fieeld,int no_bytes)
{
    static const char filename[] = "UIDSTORE.txt";
    char one_line[20];
    char temp[20];
    char *linee =  (char *)malloc(sizeof(char) * 20);
    FILE *pfilee = fopen(filename,"r");
    char *pl = one_line;
    if (pfilee != NULL)
    {
        while (fgets(one_line, sizeof(one_line), pfilee)!= NULL) {
            bzero(temp, 20);
            strcpy(temp, one_line);
            memmove (pl, pl+3, strlen (pl+3) + 1);
            if (strncmp(pl, fieeld,no_bytes)==0)
            {
                strcpy(linee, temp);
            }
        }
    }
    fclose(pfilee);
    return linee;
}



#endif /* airlinemain_h */
