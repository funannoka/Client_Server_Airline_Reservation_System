//
//  Customer_Services.c
//  tcpserver_airline_project
//
//  Created by Ifunanya Nnoka on 11/11/16.
//  Copyright © 2016 Ifunanya Nnoka. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sqlite3.h>
#include "customer_services.h"
#if defined(__APPLE__)
#define COMMON_DIGEST_FOR_OPENSSL
#define COMMON_CRYPTOR_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>
#define SHA1 CC_SHA1
#define MD5 CC_MD5
#else
#include <openssl/md5.h>
#endif

void Customer_funct(void* sock)
{
    char fromclient[2701];
    char toclient[2701];
    char depart[50];
    char arrive[50];
    char flight_[50];
    char airline[50];
    char price[50];
    char date_depart[50];
    char date_arrive[50];
    char seat_number[50];
    char cust_option;
    char username[50];
    char user_temp[50];
    char password[50];
    char *pswd_hash = NULL;
    char *user_hash = NULL;
    char first_name[50];
    char last_name[50];
    char dateofBirth[50];
    char buffa[1001];
    char local_buf[sizeof(buf)];
    char local_cb[sizeof(call_bus)];
    char *pp = local_buf;
    char *point =toclient;
    char *bp = buffa;
    int rc3;
    char temp[256],temp2[256];
    char *ptemp;
    char *encrypt;
    char *decrypt;
    
   // FILE *pointer;
    ssize_t n1;
    int num;
    int j = 0;
    int p = 0;
    int pass = 0;
    int true = 1;
    int ssock = (int) sock; /* slave server socket */
    
    union order_no{
        int i;
        char str[50];
    };
    union total_seats{
        int i;
        char str[50];
    };
    union open_seats{
        int i;
        char str[50];
    };
    
    union total_seats total_seats;
    union open_seats open_seats;
    union order_no order_no;
    /*Open Database*/
    rc3 = sqlite3_open("airline.db", &db3);
    if( rc3 ){
        fprintf(stderr, "Database failed to open: %s\n", sqlite3_errmsg(db3));
    }
    else
    fprintf(stderr, "Database opened successfully\n");
    pthread_mutex_init(&retrLock, NULL);
    pthread_mutex_init(&avLock, NULL);
    pthread_mutex_init(&seatsortLock, NULL);
    pthread_mutex_init(&seatcheckLock, NULL);
    pthread_mutex_init(&gettransLock, NULL);
    pthread_mutex_init(&orderLock, NULL);
    pthread_mutex_init(&authLock, NULL);
    pthread_mutex_init(&loginLock, NULL);
    pthread_mutex_init(&searchLock, NULL);
    pthread_mutex_init(&getInfoLock, NULL);
    pthread_mutex_init(&SearchInfoLock, NULL);
    pthread_mutex_init(&updateLock, NULL);

    //add_column();
    //create_customer_acc();
    //create_booking_table();
    int o = sqlite3_threadsafe();
    printf("sqlite_threadsafe customer: %d\n",o);
    while (true) {
    top:
        bzero(toclient,sizeof(toclient));
        printf("%s",wClientMsg);
        strcpy(toclient,Welcome_screen_cust);
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
        cust_option = fromclient[0];
        if (cust_option == '1') {
            //new user
    signup: getInfo((void*)ssock);
            strcpy(username,usr);
            strcpy(password,passw);
            strcpy(first_name,firstName);
            strcpy(last_name,lastName);
            strcpy(dateofBirth,birthday);
            pthread_mutex_unlock(&getInfoLock);
            pswd_hash = hash_gen_md5(password, strlen(password));
            user_hash = hash_gen_md5(username, strlen(username));
            p = createAccount(user_hash, pswd_hash, first_name, last_name,dateofBirth);
            free(user_hash);
            free(pswd_hash);
            if (p == 1){
                printf("%s",wClientMsg);
                point = &toclient[0];
                point += sprintf(point,"%s",username);
                point += sprintf(point,"%s",regSuccessMsg);
                encrypt = Encryypt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
            }else{
                printf("%s",wClientMsg);
                point = &toclient[0];
                bzero(temp, 256);
                bzero(temp2, 256);
                ptemp = &UsrTakenMsg[0];
                strcpy(temp, UsrTakenMsg);
                memmove(temp2, ptemp+8, strlen(ptemp+8)+1);
                memcpy(&temp[9], username, strlen(username));
                memcpy(&temp[9+strlen(username)], temp2, strlen(temp2));
                point += sprintf(point,"%s",temp);
                encrypt = Encryypt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
                goto signup;
            }
            goto login;
        }
        else if (cust_option == '2'){
            //existing user option
    login:  getUser((void*)ssock);
            strcpy(username,usr);
            strcpy(password,passw);
            pthread_mutex_unlock(&getInfoLock);
            //check if username exists in server first
            user_hash = hash_gen_md5(username, strlen(username));
            bp = &buffa[0];
            bp += sprintf(bp,"%s",slctUsrMsg);
            bp += sprintf(bp,"'%s';",user_hash);
            pthread_mutex_lock(&authLock);
            j = authenticate(buffa);
            pthread_mutex_unlock(&authLock);
            if (j == 1) {
            //user does not exist in server
                printf("%s",wClientMsg);
                point = &toclient[0];
                bzero(temp, 256);
                bzero(temp2, 256);
                ptemp = &NotExistMsg[0];
                strcpy(temp, NotExistMsg);
                memmove(temp2, ptemp+8, strlen(ptemp+8)+1);
                memcpy(&temp[9], username, strlen(username));
                memcpy(&temp[9+strlen(username)], temp2, strlen(temp2));
                point += sprintf(point,"%s",temp);
                encrypt = Encryypt(point);
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
                if (fromclient[0] == '1'){
                    goto login;
                }else if (fromclient[0] == '2'){
                    //exit flight professionals service
                    bzero(toclient,2701);
                    strcpy(toclient,customerExitMsg);
                    encrypt = Encryypt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    true = 0;
                    break;
                } else goto login;
            }
            //authenticate login details
            pswd_hash = hash_gen_md5(password, strlen(password));
            pthread_mutex_lock(&loginLock);
            j = login_user(user_hash,pswd_hash);
            pthread_mutex_unlock(&loginLock);
            if (j == 1){
                //successfully logged in
                printf("%s",wClientMsg);
                point = &toclient[0];
                bzero(temp, 256);
                bzero(temp2, 256);
                ptemp = &loginSuccessMsg[0];
                strcpy(temp, ptemp);
                memmove(temp2, ptemp+8, strlen(ptemp+8)+1);
                memcpy(&temp[8], username, strlen(username));
                memcpy(&temp[8+strlen(username)], temp2, strlen(temp2));
                point += sprintf(point,"%s",temp);
                encrypt = Encryypt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");}
                pass = 1; j = 0;
            }else{
                //login failed
                printf("%s",wClientMsg);
                bzero(toclient,2701);
                strcpy(toclient,loginFailMsg);
                encrypt = Encryypt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
                goto login;
            }
        }else goto top;
        if (pass == 1){
            pass = 0;
            printf("%s",rClientMsg);
            bzero(fromclient,sizeof(fromclient));
            n1 = read(ssock,fromclient,sizeof(fromclient));
            decrypt = Decryypt(fromclient);
            bzero(fromclient,sizeof(fromclient));
            strcpy(fromclient,decrypt);
            if (n1 < 0) {
                errexit("ERROR reading from socket %s\n", strerror(errno));
            }
            if (fromclient[0] == '1'){
        search: getSearchInfo((void*)ssock);
                strcpy(depart,from);
                strcpy(arrive,to);
                pthread_mutex_unlock(&SearchInfoLock);
                /*search flight*/
                /*Function searches for flight in database
                 with those parameters and extracts flight number(s)*/
                p=0;
                bzero(local_cb, sizeof(local_cb));
                pthread_mutex_lock(&searchLock);
                p = search_flight(depart,arrive);
                bzero(local_buf, sizeof(local_buf));
                strcpy(local_buf, buf);
                strcpy(local_cb, call_bus2);
                pthread_mutex_unlock(&searchLock);
                int _count = (strlen(local_buf)/6.0);
                if (p != 1){
                    //no flights matched
                    printf("%s",wClientMsg);
                    memset(toclient, '\0', sizeof(toclient));
                    strcpy(toclient,NoFlightMsg);
                    encrypt = Encryypt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    printf("reading from the client\n");
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt = Decryypt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    if (fromclient[0] == '1'){
                        goto search;
                    }
                }else{
                    //send search results to client
                    printf("%s",wClientMsg);
                    bzero(toclient,2701);
                    strcpy(toclient,local_cb);
                    int i;
                    encrypt = Encryypt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    printf("%s",wClientMsg);
                    //send book flight option message
                    bzero(toclient,2701);
                    bookFlightMsg(_count);
                    strcpy(toclient,msgbuf);
                    pthread_mutex_unlock(&msgLock);
                    encrypt = Encryypt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    //retrieve client flight booking choice
                    printf("%s",rClientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt = Decryypt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0){
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    if (atoi(&fromclient[0]) <=_count)
                    {//mechanism for detecting the flight picked by user
                        pp = &local_buf[0];
                        for (i=0; i<_count; i++){
                            if (atoi(&fromclient[0])== i+1){
                                strncpy(flight_,pp,6);
                            }
                            memmove(pp, pp+6, strlen(pp+6) + 1);
                        }
                        select_(0,"total_seats", "data", "name",(void*)flight_);
                        strcpy(total_seats.str, call_bus4);
                        pthread_mutex_unlock(&retrLock);
                        total_seats.i = atoi(total_seats.str);
                        select_(0,"open_seats", "data", "name",(void*)flight_);
                        strcpy(open_seats.str, call_bus4);
                        pthread_mutex_unlock(&retrLock);
                        open_seats.i = atoi(open_seats.str);
                        if (open_seats.i == 0) {
                            //flight is fully booked
                            printf("%s",wClientMsg);
                            char temp[256],temp2[50];
                            char *ptemp = fullBookedMsg;
                            strcpy(temp, fullBookedMsg);
                            memmove(temp2, ptemp+31, strlen(ptemp+31)+1);
                            memcpy(&temp[31], flight_, strlen(flight_));
                            memcpy(&temp[31+6], temp2, strlen(temp2));
                            point = &toclient[0];
                            point += sprintf(point, "%s",temp);
                            point += sprintf(point,"\n%s",airlineOption);
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
                            if (fromclient[0]== '1') {
                                goto search;
                            }else{
                            bzero(toclient,2701);
                            strcpy(toclient,customerExitMsg);
                            encrypt = Encryypt(toclient);
                            n1=write(ssock,encrypt,strlen(encrypt));
                            if (n1 <= 0){
                                errexit("ERROR writing to socket\n");
                            }
                            }true = 0;break;
                        }
                        num = total_seats.i - open_seats.i;
                        /*confirm booking*/
                        printf("%s",wClientMsg);
                        memset(toclient, '\0', sizeof(toclient));
                        strcpy(toclient,cnfrmBookinMsg);
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
                        if (fromclient[0] == '1'){
                        //complete transaction
                        //Retrieve flight and customer info for booking process
                        //check to see which seat numbers
                        //have been taken on a specific flight
                        select_(0,"airline","data","name",(void*)flight_);
                        strcpy(airline, call_bus4);
                        pthread_mutex_unlock(&retrLock);
                        select_(0,"price","data","name",(void*)flight_);
                        strcpy(price, call_bus4);
                        pthread_mutex_unlock(&retrLock);
                        select_(0,"date_depature","data","name",(void*)flight_);
                        strcpy(date_depart, call_bus4);
                        pthread_mutex_unlock(&retrLock);
                        select_(0,"date_arrival","data","name",(void*)flight_);
                        strcpy(date_arrive, call_bus4);
                        pthread_mutex_unlock(&retrLock);
                        select_(0,"First_name","customer_account","username",(void*)user_hash);
                        strcpy(first_name, call_bus4);
                        pthread_mutex_unlock(&retrLock);
                        select_(0,"Last_name","customer_account","username",(void*)user_hash);
                        strcpy(last_name, call_bus4);
                        pthread_mutex_unlock(&retrLock);
                pick:   memset(local_cb, '\0', sizeof(local_cb));
                        pthread_mutex_lock(&seatcheckLock);
                        int s = seat_crosscheck("..",flight_,seat_sort);
                        sprintf(local_cb,"\n%s\n", call_bus3);
                        pthread_mutex_unlock(&seatcheckLock);
                        if (s == 0){
                            //no seats are booked on that flight
                            printf("%s",wClientMsg);
                            point = &toclient[0];
                            point += sprintf(point,"%s",noBookedSeat);
                            point += sprintf(point," %d.",total_seats.i);
                        }else{
                pick_:      printf("%s",wClientMsg);
                            point = &toclient[0];
                            bzero(temp, 256);
                            bzero(temp2, 256);
                            ptemp = &chooseSeatMsg[0];
                            strcpy(temp, chooseSeatMsg);
                            memmove(temp2, ptemp+21, strlen(ptemp+21)+1);
                            memcpy(&temp[22], local_cb, strlen(local_cb));
                            memcpy(&temp[22+strlen(local_cb)], temp2, strlen(temp2));
                            point += sprintf(point,"%s",temp);
                            point += sprintf(point," %d.",total_seats.i);
                        }
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
                        if ((atoi(fromclient) < 1)||(atoi(fromclient) > total_seats.i)) {
                            goto pick_;
                        }
                        /* Compare client input with already reserved seats*/
                        pthread_mutex_lock(&seatcheckLock);
                        int x = seat_crosscheck(fromclient,flight_,seat_sort);
                        if (x == 1) {
                            //client picked already booked seat
                            pthread_mutex_unlock(&seatcheckLock);
                            goto pick;
                        }else{
                            //Books flight with valid seat num chosen
                            //unlocks seatcheckLock only after flight is booked
                            //gettransLock (a lock on the transaction number generator)
                            //is unlocked only after flight is booked
                            strcpy(seat_number,fromclient);
                            order_no.i = getOrderInfo("0",gettransLock, retrieve_transaction);
                            p =  BookFlightx(order_no.i, flight_, user_hash,
                            first_name, last_name, airline, depart, arrive,
                            seat_number, price, date_depart, date_arrive);
                            pthread_mutex_unlock(&seatcheckLock);
                        }
                            if (p == 1) {
                                /*Successful booking*/
                                //update available seats info in database
                                pthread_mutex_lock(&updateLock);
                                //get most updated number of open seats before update
                                select_(0,"open_seats", "data", "name",(void*)flight_);
                                strcpy(open_seats.str, call_bus4);
                                pthread_mutex_unlock(&retrLock);
                                open_seats.i = atoi(open_seats.str);
                                open_seats.i--;
                                update(5,"data","open_seats",(void*)open_seats.i,"name",flight_);
                                pthread_mutex_unlock(&updateLock);
                                printf("%s",wClientMsg);
                                point = &toclient[0];
                                point += sprintf(point,"%s",ConfirmMsg);
                                point += sprintf(point,"%d.",order_no.i);
                                point += sprintf(point,"%s",viewOrderSumm);
                                //send user a confirmation of transaction
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
                                if (fromclient[0] == '1') {
                                    //display booking details
                                    bp = &buffa[0];
                                    bp += sprintf(bp,"%s",displayBookinSyntax);
                                    bp += sprintf(bp, "order_no = %d;",order_no.i);
                                    bzero(local_cb, sizeof(local_cb));
                                    pthread_mutex_lock(&avLock);
                                    add_view(buffa);
                                    sprintf(local_cb,"\n%s", call_bus);
                                    pthread_mutex_unlock(&avLock);
                                    /*Sending Transaction Details*/
                                    printf("%s",wClientMsg);
                                    bzero(toclient,2701);
                                    strcpy(toclient,local_cb);
                                    encrypt = Encryypt(toclient);
                                    n1=write(ssock,encrypt,strlen(encrypt));
                                    if (n1 <= 0){
                                        errexit("ERROR writing to socket\n");
                                    }
                                }
                            }
                            else{
                                /*Unable to book*/
                                printf("%s",wClientMsg);
                                bzero(toclient,2701);
                                strcpy(toclient,bookingErrorMsg);
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
                                if (fromclient[0] == '1') {
                                    goto search;
                                }else{
                                    printf("%s",wClientMsg);
                                    bzero(toclient,2701);
                                    strcpy(toclient,customerExitMsg);
                                    encrypt = Encryypt(toclient);
                                    n1=write(ssock,encrypt,strlen(encrypt));
                                    if (n1 <= 0){
                                        errexit("ERROR writing to socket\n");
                                    }
                                    true = 0;
                                    break;
                                }
                            }
                        }
                        else if (fromclient[0] == '2'){
                            /*cancel*/
                            /*exit or go to search*/
                            printf("%s",wClientMsg);
                            bzero(toclient,2701);
                            strcpy(toclient,airlineOption);
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
                            if (fromclient[0] == '1') {
                                goto search;
                            }
                            else
                            {   printf("%s",wClientMsg);
                                bzero(toclient,2701);
                                strcpy(toclient,customerExitMsg);
                                encrypt = Encryypt(toclient);
                                n1=write(ssock,encrypt,strlen(encrypt));
                                if (n1 <= 0){
                                    errexit("ERROR writing to socket\n");
                                }
                            }true = 0;break;
                        }
                    }else if(atoi(&fromclient[0]) ==(_count+1)) goto search;
                }
            }else if (fromclient[0] == '2'){
            view://view bookings
                /*Request for source*/
                printf("%s",wClientMsg);
                bzero(toclient,2701);
                strcpy(toclient,viewBookingOptMsg);
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
                if (fromclient[0] == '1'){
                //Use transaction number
                    printf("%s",wClientMsg);
                    bzero(toclient,2701);
                    strcpy(toclient,orderNumQuery);
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
                    strcpy(order_no.str, fromclient);
                    //check if transaction number exists
                    //and also if it matches user
                    j = getOrderInfo(order_no.str,orderLock,confirm_order_);
                    if (j != 1) {
                    /*Transaction number does not match our records*/
                    printf("%s",wClientMsg);
                    bzero(toclient,2701);
                    strcpy(toclient,noRecordsMsg);
                    encrypt = Encryypt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                        goto view;
                    }else{
                        order_no.i = atoi(order_no.str);
                        bzero(buffa, 1001);
                        select_(2, "user", "booked_flights", "order_no", (void*)order_no.i);
                        strcpy(user_temp, call_bus4);
                        pthread_mutex_unlock(&retrLock);
                        if (strcmp(user_temp, user_hash)!= 0){
                        /*Transaction number assigned to a different client*/
                            printf("%s",wClientMsg);
                        bzero(toclient,2701);
                        strcpy(toclient,invalidOrderMsg);
                        encrypt = Encryypt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }goto view;
                    }
                }
                    bp = &buffa[0];
                    bp += sprintf(bp,"%s",displayBookinSyntax);
                    bp += sprintf(bp, "order_no = %d;",order_no.i);
                    bzero(local_cb, sizeof(local_cb));
                    pthread_mutex_lock(&avLock);
                    j = add_view(buffa);
                    sprintf(local_cb,"%s", call_bus);
                    pthread_mutex_unlock(&avLock);
                    /*Sending Transaction Details*/
                    printf("%s",wClientMsg);
                    bzero(toclient,2701);
                    sprintf(toclient,"\n%s",local_cb);
                    encrypt = Encryypt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                }else if (fromclient[0] == '2'){
                    //view via login
                    bp = &buffa[0];
                    bp += sprintf(bp,"%s",displayBookinSyntax);
                    bp += sprintf(bp, "user = '%s';",user_hash);
                    pthread_mutex_lock(&authLock);
                    j = authenticate(buffa);
                    pthread_mutex_unlock(&authLock);
                    if (j == 1) {
                        j=0;
                        printf("%s",wClientMsg);
                        bzero(toclient,2701);
                        sprintf(toclient,noReserveMsg,username);
                        encrypt = Encryypt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                    }else{
                        bzero(local_cb, sizeof(local_cb));
                        pthread_mutex_lock(&avLock);
                        j = add_view(buffa);
                        sprintf(local_cb,"%s", call_bus);
                        pthread_mutex_unlock(&avLock);
                        /*Sending Transaction Details*/
                        printf("%s",wClientMsg);
                        bzero(toclient,2701);
                        sprintf(toclient,"\n%s",local_cb);
                        encrypt = Encryypt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                    }
                }
                /*exit , cancel booking or go to search*/
   options:     printf("%s",wClientMsg);
                bzero(toclient,2701);
                strcpy(toclient,bookedflightOpt);
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
                if (fromclient[0] == '2'){
                /*cancel booking*/
                    printf("%s",wClientMsg);
                    bzero(toclient,2701);
                    strcpy(toclient,orderNumQuery);
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
                    strcpy(order_no.str, fromclient);
                    //check if transaction number exists and also if it matches user
                    j = getOrderInfo(order_no.str,orderLock,confirm_order_);
                    if (j != 1) {
                        /*Transaction number does not match our records*/
                        printf("%s",wClientMsg);
                        bzero(toclient,2701);
                        strcpy(toclient,noRecordsMsg);
                        encrypt = Encryypt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                        goto options;
                    }else{
                        order_no.i = atoi(order_no.str);
                        bzero(buffa, 1001);
                        select_(2, "user", "booked_flights", "order_no", (void*)order_no.i);
                        strcpy(user_temp, call_bus4);
                        pthread_mutex_unlock(&retrLock);
                        if (strcmp(user_temp, user_hash)!= 0){
                            /*Transaction number assigned to a different client*/
                            printf("%s",wClientMsg);
                            bzero(toclient,2701);
                            strcpy(toclient,invalidOrderMsg);
                            encrypt = Encryypt(toclient);
                            n1=write(ssock,encrypt,strlen(encrypt));
                            if (n1 <= 0){
                                errexit("ERROR writing to socket\n");
                            }goto options;
                        }
                    }
                    //confirm cancel booking
                    printf("%s",wClientMsg);
                    bzero(toclient,2701);
                    strcpy(toclient,confirmCancelMsg);
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
                    if (fromclient[0] == '1') {
                    /*now cancel booking*/
     cancelOrder:       //get flight name/number before cancellation for later use
                        sprintf(order_no.str, "%d",order_no.i);
                        select_(1,"flight", "booked_flights", "order_no",(void*)order_no.str);
                        strcpy(flight_, call_bus4);
                        pthread_mutex_unlock(&retrLock);
                        p = cancelBooking(order_no.str);
                        if (p == 1) {
                        /*increase available seats in flight by 1*/
                            pthread_mutex_lock(&updateLock);
                            //get most updated number of open seats before update
                            select_(0,"open_seats", "data", "name",(void*)flight_);
                            strcpy(open_seats.str, call_bus4);
                            pthread_mutex_unlock(&retrLock);
                            open_seats.i = atoi(open_seats.str);
                            open_seats.i++;
                            update(5,"data","open_seats",(void*)open_seats.i,"name",flight_);
                            pthread_mutex_unlock(&updateLock);
                            printf("%s",wClientMsg);
                            bzero(toclient,2701);
                            strcpy(toclient,cancelConfirmMsg);
                            encrypt = Encryypt(toclient);
                            n1=write(ssock,encrypt,strlen(encrypt));
                            if (n1 <= 0){
                                errexit("ERROR writing to socket\n");
                            }
                        }else{
                            printf("%s",wClientMsg);
                            bzero(toclient,2701);
                            strcpy(toclient,cancelBookingErrMsg);
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
                            if (fromclient[0] == '1'){
                                goto cancelOrder;
                            }else goto exit;
                        }
                    }else goto exit;
                }else if (fromclient[0] == '3') goto exit;
                 else if (fromclient[0] == '1') goto search;
            }else{
                /*exit or go back to correct entry*/
                printf("%s",wClientMsg);
                bzero(toclient,2701);
                strcpy(toclient,finalInstanceMsg);
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
                if (fromclient[0] == '1') goto search;
                else if (fromclient[0] == '2')goto view;
                else if (fromclient[0] == '3'){
                    printf("%s",wClientMsg);
    exit:           bzero(toclient,2701);
                    strcpy(toclient,customerExitMsg);
                    encrypt = Encryypt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    true = 0;
                }
            }
        }
        free(user_hash);
        free(pswd_hash);
    }
}


