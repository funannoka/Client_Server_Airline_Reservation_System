
//
//  customer.h
//  tcpserver_airline_project
//
//  Created by Ifunanya Nnoka on 11/23/16.
//  Copyright © 2016 Ifunanya Nnoka. All rights reserved.
//

#ifndef customer_h
#define customer_h
#if defined(__APPLE__)
#define COMMON_DIGEST_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>
#define SHA1 CC_SHA1
#define SHA512 CC_SHA1
#define MD5 CC_MD5
#else
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/md5.h>
#endif

extern int errno;
void Customer_funct(void* sock);
int errexit(const char *format, ...);
char * extract_line(char *field,const char filename[],int no_of_bytes);
void create_customer_acc(void);
void create_booking_table(void);
int createAccount(char *user,char *pswd, char *firname, char *lstname, char *bday);
int login_user(char* user,char* password);
int search_flight (const char *source, const char *dest);
int add_view(char* buffer);
void add_column(void);
int confirm_order_(char* t_id,char* buffer);
int seat_crosscheck(char *num,char *flight,
int sort_dup_check(void *data, int argc, char **argv, char **azColName));
void retrieve(char *buffer);
int authenticate(char* buffer);
int retrieve_transaction(char* NotUsed,char *buffer);
int seat_sort(void *data, int argc, char **argv, char **azColName);
int seat_dup_check(void *filename, int argc, char **argv, char **azColName);
int _user_authenticate(char* user,char* password);
int getOrderInfo(char *field,pthread_mutex_t mLockk,int function(char* n,char* buffer));
int BookFlightx(int transaction_num,char *flight,char *username,char *first_name,
char *last_name,char *airline,char *depart,char *arrive,char *seatNum,
char *price,char *date_depart,char *date_arrive);
void update(int i,char table[],char field1[],void  *one,char field2[],char *two);
void getInfo (void* sock);
void getUser(void* sock);
void getSearchInfo(void* sock);
void bookFlightMsg(int count);
int cancelBooking(const char *orderNum);
void select_(int i,char field[],char table[],char primaryKey[],void *key);
char *hash_gen_md5(const char *field, int len);

sqlite3 *db3;
int ok,k,id_k,f_ok,seat_ok;
char buf[256];
char msgbuf[256];
char call_bus[2701];
char call_bus2[2701];
char call_bus3[2701];
char call_bus4[2701];
char *cb2 = call_bus;
char *cb3 = call_bus2;
char *cb4 = call_bus3;
char *cb5 = call_bus4;
char usr[50];
char passw[50];
char firstName[50];
char lastName[50];
char birthday[50];
char from[50];
char to[50];
int order_ok = 0;
int not_exist = 0;
int f_count = 0;
int tst;
int no_of_flights=0;
char *pp = buf;
pthread_mutex_t retrLock,getInfoLock,avLock;
pthread_mutex_t seatsortLock,seatcheckLock;
pthread_mutex_t gettransLock,orderLock;
pthread_mutex_t loginLock,searchLock;
pthread_mutex_t authLock,SearchInfoLock;
pthread_mutex_t msgLock,updateLock;
char Welcome_screen_cust[] = "Welcome to Airline reservation."\
"Are you registered with us?\n 1. New User 2. Existing User ";
char ConfirmMsg[]="\nCongratulations! Your flight reservations have been made.\nYour Order Number is ";
char regSuccessMsg[] = " you have been successfully registered.\n"\
"Press Enter to login.";
char UsrTakenMsg[] = "Username not available. Please choose a different username.\n";
char slctUsrMsg[] = "SELECT * from customer_account where username = ";
char airlineOption[] = "Enter <<:: 1. Search Flights ::>>  <<:: 2. Exit ::>>";
char NotExistMsg[] = "Username does not exist in our server!!\n"\
"Enter '1' to use a different username '2' to exit";
char loginSuccessMsg[] = "Welcome ! What would you like to do?\n"\
"<<:: 1. Search Flights ::>>  <<:: 2. View bookings ::>> ";
char loginFailMsg[] = "Incorrect password!";
char NoFlightMsg[] = "No flights available through that route.\n"\
"Search more flights?\n Enter <<:: 1. Search Flights ::>>  <<:: 2. Exit ::>>";
char cnfrmBookinMsg[] = "Enter <<:: 1. Complete Transaction ::>>  <<:: 2. Cancel ::>> ";
char chooseSeatMsg[] = "Seats already booked: Choose a Seat Number to Book Between Seat 1 - Seat";
char noBookedSeat[] = "All Seats are Available.\nEnter a Seat Number between Seat 1 - Seat"; 
char viewOrderSumm[] = "\nEnter 1 to view your flight reservations.";
char displayBookinSyntax[] = "SELECT order_no AS 'Transaction No.',Flight AS 'Flight No.', First_name "\
"AS 'First Name',Last_name AS 'Last Name',Airline,Source AS 'From',Destination AS 'To',seat_number "\
"AS 'Seat No.',Date_depart AS 'Date of Depature',Date_arrive AS 'Date of Arrival',price "\
"from booked_flights where ";
char bookingErrorMsg[] = "We're sorry! There was an error while booking your flight.\n"\
"Enter 1 to search more flights or any character to exit";
char viewBookingOptMsg[] = "How would you like to retrieve your booking information?\n"\
" <<:: 1. View By Order No. ::>> <<:: 2. View All Bookings ::>>";
char orderNumQuery[] = "Enter your order number ";
char noRecordsMsg[] = "Order number not in our records.\n Please Enter a valid order number.";
char invalidOrderMsg[] = "Order number does not match your records";
char noReserveMsg[] = "Looks like you have made no reservations with us";
char finalInstanceMsg[] = "Incorrect Entry!\n <<:: 1. Search Flights ::>> "\
"<<:: 2. View Bookings ::>> <<:: 3. Exit. ::>>";
char fullBookedMsg[] = "Unfortunately, Seats on flight  are fully booked.";
char bookedflightOpt[] = "Enter <<:: 1. Search Flights ::>>  <<:: 2. Cancel Booking ::>> <<:: 3. Exit ::>> ";
char cancelBookingErrMsg[] = "There was an error while cancelling your reservation.\n"\
"<<:: 1. Try again ::>>  <<:: 2. Exit ::>>  ";
char confirmCancelMsg[] = "<<:: 1. Confirm Cancellation ::>>  <<:: 2. Exit ::>> ";
char cancelConfirmMsg[] = "Flight reservation has been successfully cancelled.";
char customerExitMsg[] = "Goodbye!";
char fromLocationMsg[] = "Enter Departure Location ";
char toLocationMsg[] = "Enter Arrival Location ";
char wClientMsg[] = "writing to the client\n";
char rClientMsg[] = "reading from the client\n";
char usernQueryMsg[] = "Enter new username ";
char pswnQueryMsg[] = "Enter new password ";
char userQueryMsg[] = "Enter username ";
char pswQueryMsg[] = "Enter password ";
char nameQueryMsg1[] = "Enter First name ";
char nameQueryMsg2[] = "Enter Last name ";
char bdayQueryMsg[] = "Enter Date of Birth ";

int mainCallback(void *data, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        cb2 += sprintf(cb2,"%s: %s\n",azColName[i], argv[i] ? argv[i] : "NULL");
        if (argv[i] == NULL) {
            ok = 0;
        }else ok = 1;
    }
    cb2 += sprintf(cb2,"\n");
    return 0;
}

int auth_callback(void *data, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        //if null no username matched client's
        if (argv[i] == NULL) {
            not_exist = 1;
        }else not_exist = 0;
    }
    return 0;
}

int callback2(void *password,int argc,char **argv,char **azColName){
    int i;
    id_k = 0;
    for(i=0; i<argc; i++){
        if(strcmp((const char*)password,argv[i])==0){
            id_k = 1;
        }
    }
    return 0;
}

int callback3(void *data, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        if(strcmp((const char*) data,argv[i])==0){
            order_ok = 1;
        }
    }
    return 0;
}

int flight_identifier(void *data, int argc, char **argv, char **azColName){
    int i;
    f_count++;
    //counts how many flight matched the client's search parameters
    cb3 += sprintf(cb3,"\n<<<<++++FLIGHT %d.++++>>>>\n",f_count);
    for(i=0; i<argc; i++){
        //extracts flight parameters if there is a match
        cb3 += sprintf(cb3,"%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        //used to extract flight number/name parameter
        if (strncmp("NAME", azColName[i], 4)==0) {
            pp +=sprintf(pp,"%s", argv[i]);
        }
        //if null no flight matched client's search parameter
        if (argv[i] == NULL) {
            f_ok = 0;
        }else f_ok = 1;
    }
    cb3 += sprintf(cb3,"\n");
    return 0;
}

int retr(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        sprintf(call_bus4,"%s",argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

int retr_trans(void *NotUsed, int argc, char **argv, char **azColName){
    //generates transaction number
    int i;
    for(i=0; i<argc; i++){
        if (k <= atoi(argv[i]) ){
            k = atoi(argv[i]);
            k++;
        }
    }
    return 0;
}

int seat_dup_check(void *filename, int argc, char **argv, char **azColName){
    int i;
    FILE *fp;
    int stdout_bk; //fd for stdout backup
    stdout_bk = dup(fileno(stdout));
    fp = fopen(filename, "a");
    dup2(fileno(fp), fileno(stdout));
    for (i=0; i<argc; i++) {
        printf("%s",argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    dup2(stdout_bk, fileno(stdout)); //restore
    fclose(fp);
    return 0;
}


int seat_sort(void *data, int argc, char **argv, char **azColName){
    //gets data on all booked seats in flight and returns to client
    //also checks to see if client picked an already booked seat
    int i;
    tst = 1;
    for(i=0; i<argc; i++){
        cb4 += sprintf(cb4," %s,",argv[i] ? argv[i] : "NULL");
        if(argv[i] == NULL){
            seat_ok = 0;
            tst = 0;
        }else if(strcmp((const char*) data,argv[i])==0){
            //client picked an already booked seat
            seat_ok = 1;
        }
    }
    return 0;
}


void create_customer_acc(void)
{
    char *sql;
    char *zErrMsg = 0;
    int rc;
    /*Creates SQL statement*/
    sql = "Create table customer_account("\
    "username char primary key not null,"\
    "password          text not null,"\
    "First_name        text not null,"\
    "Last_name         text not null,"\
    "Date_of_Birth     text not null);";

    /*Execute SQL statement*/
    rc = sqlite3_exec(db3, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
}

void create_booking_table(void)
{
    char *sql;
    char *zErrMsg = 0;
    int rc;
    
    /*Creates SQL statement*/
    sql = "Create table booked_flights("\
    "order_no  int primary key not null,"\
    "Flight       text not null,"\
    "user         text not null,"\
    "First_name   text not null,"\
    "Last_name    text not null,"\
    "Airline      text not null,"\
    "Source       text not null,"\
    "Destination  text not null,"\
    "seat_number  int  not null,"\
    "Price        real not null,"\
    "Date_depart  text not null,"\
    "Date_arrive  text not null);";
    /*Execute SQL statement*/
    rc = sqlite3_exec(db3, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
}

void add_column(void)
{
    char *sql;
    char *zErrMsg = 0;
    int rc;
    
    sql ="ALTER TABLE booked_flights ADD COLUMN Date_depart char;";
    /*Execute SQL statement*/
    rc = sqlite3_exec(db3, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        //   fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sql ="ALTER TABLE booked_flights ADD COLUMN Date_arrive char;";
    /*Execute SQL statement*/
    rc = sqlite3_exec(db3, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        //   fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

int add_view(char* buffer)
{
    ok = 0;
    char *zErrMsg = 0;
    int rc;
    cb2 = &call_bus[0];
    char *sql;
    /*Creates SQL statement. Inserts information into SQL table*/
    sql = buffer;
    /*Execute SQL statement*/
    rc = sqlite3_exec(db3, sql, mainCallback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
            ok =1;
    }
    return ok;
}

int authenticate(char* buffer)
{
    not_exist = 1;
    int rc;
    char *zErrMsg = 0;
    char *sql;
    /*Creates SQL statement. */
    sql = buffer;
    /*Execute SQL statement*/
    rc = sqlite3_exec(db3, sql, auth_callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return not_exist;
}

int login_user(char* user,char* password)
{
    char *zErrMsg = 0;
    char *sql;
    char buff[256];
    int rc;
    
    bzero(buff, 256);
    /*Creates SQL statement.*/
    sprintf(buff, "SELECT password from customer_account where username = '%s';",user);
    sql = buff;
    /*Execute SQL statement*/
    rc = sqlite3_exec(db3,sql,callback2,(void*)password,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return id_k;
}


int search_flight (const char *source, const char *dest)
{
    f_ok = 0;
    f_count=0;
    cb3 = &call_bus2[0];
    pp = &buf[0];
    int rc;
    char *zErrMsg = 0;
    char buff[256];
    char *sql;
    bzero(buff, 256);
    const char filename[] = "search_flight.txt";
    sprintf(buff, "SELECT name,airline,source,destination,price,open_seats AS'OPEN SEATS',"\
            "DATE_DEPATURE AS 'Date of Departure',DATE_ARRIVAL AS 'Date of Arrival'"\
            "from data where source = '%s' AND destination = '%s';",source,dest);
    sql = buff;
    /*Execute SQL statement*/
    rc = sqlite3_exec(db3, sql, flight_identifier, (void*)filename, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        f_ok = 0;
    }
    return f_ok;
}

int seat_crosscheck(char *num,char *flight,int sort_dup_check(void *data, int argc, char **argv, char **azColName)){
    char *zErrMsg = 0;
    cb4 = &call_bus3[0];
    char buff[256];
    seat_ok = 2;
    tst = 0;
    int rc;
    char *sql;
    bzero(buff, 256);
    //check to see which seat numbers have been taken on a specific flight
    sprintf(buff, "SELECT seat_number from booked_flights where flight = '%s';",flight);
    sql = buff;
    /*Execute SQL statement*/
    rc = sqlite3_exec(db3, sql,sort_dup_check, num, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else if (tst == 0){
        seat_ok = 0;
    }
    return seat_ok;
}
int confirm_order_(char* t_id,char* buffer)
{
    char *zErrMsg = 0;
    char *sql;
    int rc;
    order_ok = 0;
    sql = buffer;
    /*Execute SQL statement*/
    rc = sqlite3_exec(db3, sql, callback3,(void*)t_id, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return order_ok;
}

void retrieve(char *buffer)
{
    char *zErrMsg = 0;
    char *sql;
    int rc;
    cb5 = &call_bus4[0];
    /*Execute SQL statement*/
    sql = buffer;
    rc = sqlite3_exec(db3, sql, retr,0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}
int retrieve_transaction(char *NotUsed,char *buffer)
{
    k=1;
    int rc;
    char *zErrMsg = 0;
    char *sql;
    sql = buffer;
    /*Execute SQL statement*/
    rc = sqlite3_exec(db3, sql, retr_trans,0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return k;
}

void select_(int i,char field[],char table[],char primaryKey[],void *key){
    char buffa[1001];
    bzero(buffa, 1001);
    if(i == 0){
        char *k = (char*)key;
        sprintf(buffa, "SELECT %s from %s where %s = '%s';",field,table,primaryKey,k);
    }else if (i == 1){
        char *k = (char*)key;
        sprintf(buffa, "SELECT %s from %s where %s = %s;",field,table,primaryKey,k);
    }else if (i == 2){
        int k = (int)key;
        sprintf(buffa, "SELECT %s from %s where %s = %d;",field,table,primaryKey,k);
    }
    pthread_mutex_lock(&retrLock);
    retrieve(buffa);
        
}
int getOrderInfo(char field[],pthread_mutex_t mLockk,int function(char* n,char* buffer)){
    char buffa[1001];
    int returnval = 0;
    bzero(buffa, 1001);
    sprintf(buffa, "SELECT order_no from booked_flights;");
    pthread_mutex_lock(&mLockk);
    returnval = function(field,buffa);
    pthread_mutex_unlock(&mLockk);
    return returnval;
}

int BookFlightx(int transaction_num,char *flight,char *username,char *first_name,
char *last_name,char *airline,char *depart,char *arrive,char *seatNum,char *price,
char *date_depart,char *date_arrive){
    char buffa[1001];
    int p=0;
    bzero(buffa, 1001);
    sprintf(buffa, "insert into booked_flights values"\
            "(%d, '%s','%s','%s','%s','%s','%s','%s',%s,%s,'%s','%s');",
            transaction_num,flight,username,first_name,last_name,airline,
            depart,arrive,seatNum,price,date_depart,date_arrive);
    pthread_mutex_lock(&avLock);
    p = add_view(buffa);
    pthread_mutex_unlock(&avLock);
    return p;
}
void update(int i,char table[],char field1[],void *one,char field2[],char *two){
    char buffa[1001];
    bzero(buffa, 1001);
    if(i == 1){
        char *change = (char*)one;
        sprintf(buffa, "UPDATE %s set %s"\
        "= %s where %s = '%s'",table,field1,change,field2, two);
    }else if (i == 0){
        char *change = (char*)one;
        sprintf(buffa, "UPDATE %s set %s"\
        "= '%s' where %s = '%s'",table,field1,change,field2, two);
    }else {
        int change = (int)one;
        sprintf(buffa, "UPDATE %s set %s"\
        "= %d where %s = '%s'",table,field1,change,field2, two);
    }
    pthread_mutex_lock(&avLock);
    add_view(buffa);
    pthread_mutex_unlock(&avLock);
}
int createAccount(char *user,char *pswd, char *firname, char *lstname, char *bday){
    char buffa[1001];
    int p;
    bzero(buffa, 1001);
    sprintf(buffa, "insert into customer_account values"\
            "('%s', '%s','%s','%s','%s');",user,pswd,firname,lstname,bday);
    pthread_mutex_lock(&avLock);
    p = add_view(buffa);
    pthread_mutex_unlock(&avLock);
    return p;
}

char * Encryypt(char *string)
{
    char *linee =  (char *)malloc(sizeof(char) * 2701);
    
    int n=strlen(string);
    int i=0;
    for(i=0;i<n;i++){
        string[i] = string[i]+25;
    }
    //	printf("%s\n",string);
    strcpy(linee, string);
    return linee;
    
}

char * Decryypt (char *string)
{
    char *linee =  (char *)malloc(sizeof(char) * 50);
    int n=strlen(string);
    int i=0;
    for(i=0;i<n;i++){
        string[i] = string[i]-25;
    }
    strcpy(linee, string);
    return linee;
}



void getInfo (void* sock){
    
    char toclient[2701];
    char fromclient[2701];
    char user_name[50];
    char pswd[50];
    char firstname[50];
    char lastname[50];
    char bday[50];
    char *encrypt;
    char *decrypt;
    int ssock = (int)sock;
    ssize_t n1;
    
    /*Request for username*/
    printf("%s",wClientMsg);
    bzero(toclient,2701);
    strcpy(toclient,usernQueryMsg);
    encrypt = Encryypt(toclient);
    n1=write(ssock,encrypt,strlen(encrypt));
    if (n1 <= 0){
        errexit("ERROR writing to socket\n");
    }
    printf("%s",rClientMsg);
    bzero(fromclient,sizeof(fromclient));
    n1 = read(ssock,fromclient,sizeof(fromclient));
    decrypt = Decryypt(fromclient);
    bzero(fromclient,sizeof(fromclient));
    strcpy(fromclient,decrypt);
    if (n1 < 0) {
        errexit("ERROR reading from socket %s\n", strerror(errno));
    }
    strcpy(user_name,fromclient);
    /*Request for password*/
    printf("%s",wClientMsg);
    bzero(toclient,2701);
    strcpy(toclient,pswnQueryMsg);
    encrypt = Encryypt(toclient);
    n1=write(ssock,encrypt,strlen(encrypt));
    if (n1 <= 0){
        errexit("ERROR writing to socket\n");
    }
    printf("%s",rClientMsg);
    bzero(fromclient,sizeof(fromclient));
    n1 = read(ssock,fromclient,sizeof(fromclient));
    decrypt = Decryypt(fromclient);
    bzero(fromclient,sizeof(fromclient));
    strcpy(fromclient,decrypt);
    if (n1 < 0) {
        errexit("ERROR reading from socket %s\n", strerror(errno));
    }
    strcpy(pswd,fromclient);
    /*Request for First name*/
    printf("%s",wClientMsg);
    bzero(toclient,2701);
    strcpy(toclient,nameQueryMsg1);
    encrypt = Encryypt(toclient);
    n1=write(ssock,encrypt,strlen(encrypt));
    if (n1 <= 0){
        errexit("ERROR writing to socket\n");
    }
    printf("%s",rClientMsg);
    bzero(fromclient,sizeof(fromclient));
    n1 = read(ssock,fromclient,sizeof(fromclient));
    decrypt = Decryypt(fromclient);
    bzero(fromclient,sizeof(fromclient));
    strcpy(fromclient,decrypt);
    if (n1 < 0) {
        errexit("ERROR reading from socket %s\n", strerror(errno));
    }
    strcpy(firstname,fromclient);
    /*Request for Last name*/
    printf("%s",wClientMsg);
    bzero(toclient,2701);
    strcpy(toclient,nameQueryMsg2);
    encrypt = Encryypt(toclient);
    n1=write(ssock,encrypt,strlen(encrypt));
    if (n1 <= 0){
        errexit("ERROR writing to socket\n");
    }
    printf("%s",rClientMsg);
    bzero(fromclient,sizeof(fromclient));
    n1 = read(ssock,fromclient,sizeof(fromclient));
    decrypt = Decryypt(fromclient);
    bzero(fromclient,sizeof(fromclient));
    strcpy(fromclient,decrypt);
    if (n1 < 0) {
        errexit("ERROR reading from socket %s\n", strerror(errno));
    }
    strcpy(lastname,fromclient);
    /*Request for Date of birth*/
    printf("%s",wClientMsg);
    bzero(toclient,2701);
    strcpy(toclient,bdayQueryMsg);
    encrypt = Encryypt(toclient);
    n1=write(ssock,encrypt,strlen(encrypt));
    if (n1 <= 0){
        errexit("ERROR writing to socket\n");
    }
    printf("%s",rClientMsg);
    bzero(fromclient,sizeof(fromclient));
    n1 = read(ssock,fromclient,sizeof(fromclient));
    decrypt = Decryypt(fromclient);
    bzero(fromclient,sizeof(fromclient));
    strcpy(fromclient,decrypt);
    if (n1 < 0) {
        errexit("ERROR reading from socket %s\n", strerror(errno));
    }
    strcpy(bday,fromclient);
    
    pthread_mutex_lock(&getInfoLock);
    bzero(usr, sizeof(usr));
    bzero(passw, sizeof(passw));
    bzero(firstName, sizeof(firstName));
    bzero(lastName, sizeof(lastName));
    bzero(birthday, sizeof(birthday));
    strcpy(usr,user_name);
    strcpy(passw,pswd);
    strcpy(firstName,firstname);
    strcpy(lastName,lastname);
    strcpy(birthday,bday);

    return;
}


void getUser(void* sock){
    int ssock = (int)sock;
    char toclient[2701];
    char fromclient[2701];
    char username[50];
    char password[50];
    char *enncrypt;
    char *decrypt;
    ssize_t n1;
    
    /*Request for username*/
    printf("%s",wClientMsg);
    bzero(toclient,2701);
    strcpy(toclient,userQueryMsg);
    enncrypt = Encryypt(toclient);
    n1=write(ssock,enncrypt,strlen(enncrypt));
    if (n1 <= 0){
        errexit("ERROR writing to socket\n");
    }
    printf("%s",rClientMsg);
    bzero(fromclient,sizeof(fromclient));
    n1 = read(ssock,fromclient,sizeof(fromclient));
    decrypt = Decryypt(fromclient);
    bzero(fromclient,sizeof(fromclient));
    strcpy(fromclient,decrypt);
    if (n1 < 0) {
    errexit("ERROR reading from socket %s\n", strerror(errno));
    }
    strcpy(username,fromclient);
    /*Request for password*/
    printf("%s",wClientMsg);
    bzero(toclient,2701);
    strcpy(toclient,pswQueryMsg);
    enncrypt = Encryypt(toclient);
    n1=write(ssock,enncrypt,strlen(enncrypt));
    if (n1 <= 0){
        errexit("ERROR writing to socket\n");
    }
    printf("%s",rClientMsg);
    bzero(fromclient,sizeof(fromclient));
    n1 = read(ssock,fromclient,sizeof(fromclient));
    decrypt = Decryypt(fromclient);
    bzero(fromclient,sizeof(fromclient));
    strcpy(fromclient,decrypt);
    if (n1 < 0) {
        errexit("ERROR reading from socket %s\n", strerror(errno));
    }
    strcpy(password,fromclient);
    pthread_mutex_lock(&getInfoLock);
    bzero(usr, sizeof(usr));
    bzero(passw, sizeof(passw));
    strcpy(usr,username);
    strcpy(passw,password);
}
void getSearchInfo(void* sock){
    int ssock = (int)sock;
    char toclient[2701];
    char fromclient[2701];
    ssize_t n1;
    char depart[50];
    char arrive[50];
    char *enncrypt;
    char *decrypt;
    /*Request for source*/
    printf("%s",wClientMsg);
    bzero(toclient,2701);
    strcpy(toclient,fromLocationMsg);
    enncrypt = Encryypt(toclient);
    n1=write(ssock,enncrypt,strlen(enncrypt));
    if (n1 <= 0){
        errexit("ERROR writing to socket\n");
    }
    printf("%s",rClientMsg);
    bzero(fromclient,sizeof(fromclient));
    n1 = read(ssock,fromclient,sizeof(fromclient));
    decrypt = Decryypt(fromclient);
    bzero(fromclient,sizeof(fromclient));
    strcpy(fromclient,decrypt);
    if (n1 < 0) {
        errexit("ERROR reading from socket %s\n", strerror(errno));
    }
    strcpy(depart,fromclient);
    /*Request for destination*/
    printf("%s",wClientMsg);
    bzero(toclient,2701);
    strcpy(toclient,toLocationMsg);
    enncrypt = Encryypt(toclient);
    n1=write(ssock,enncrypt,strlen(enncrypt));
    if (n1 <= 0){
        errexit("ERROR writing to socket\n");
    }
    printf("%s",rClientMsg);
    bzero(fromclient,sizeof(fromclient));
    n1 = read(ssock,fromclient,sizeof(fromclient));
    decrypt = Decryypt(fromclient);
    bzero(fromclient,sizeof(fromclient));
    strcpy(fromclient,decrypt);
    if (n1 < 0) {
        errexit("ERROR reading from socket %s\n", strerror(errno));
    }
    strcpy(arrive,fromclient);
    
    pthread_mutex_lock(&SearchInfoLock);
    bzero(from, sizeof(from));
    bzero(to, sizeof(to));
    strcpy(from,depart);
    strcpy(to,arrive);
}

void bookFlightMsg(int count)
{
    int i;
    char msg[256];
    char *point = msg;
    for (i=0; i<count; i++){
    point += sprintf(point,"Enter %d. :::>> Book Flight %d\n",i+1,i+1);
    }
    point += sprintf(point,"Enter %d. :::>> Search More Flights",i+1);
    pthread_mutex_lock(&msgLock);
    bzero(msgbuf, sizeof(msgbuf));
    strcpy(msgbuf,msg);
}

int cancelBooking(const char *orderNum)
{
    int n = 0;
    char *sql;
    char buff[256];
    int rc;
    char *zErrMsg = 0;
    
    bzero(buff, 256);
    /* Create DELETE SQL statement */
    sprintf(buff,"DELETE from booked_flights where order_no = %s;",orderNum);
    sql = buff;
    /*Execute SQL statement*/
    rc = sqlite3_exec(db3, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        n = 1;
    }
       return n;
}

char *hash_gen_md5(const char *field, int len){
    int i;
    CC_MD5_CTX p;
    unsigned char digest[CC_MD5_DIGEST_LENGTH];
    char *hash = (char*)malloc(33);
    
    CC_MD5_Init(&p);
    while (len > 0) {
        if (len > 512) {
            CC_MD5_Update(&p, field, 512);
        } else {
            CC_MD5_Update(&p, field, len);
        }
        len -= 512;
        field += 512;
    }
    CC_MD5_Final(digest, &p);
    for (i = 0; i < sizeof(digest); ++i) {
        snprintf(&(hash[i*2]),CC_MD5_DIGEST_LENGTH*2,"%02x",(unsigned int)digest[i]);
    }
    return hash;
}


#endif /* customer_h */
