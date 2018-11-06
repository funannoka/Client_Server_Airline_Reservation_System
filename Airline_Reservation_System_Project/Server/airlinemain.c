//  main.c
//  tcpserver_airline_project
//
//  Created by Ifunanya Nnoka on 10/28/16.
//  Copyright © 2016 Ifunanya Nnoka. All rights reserved.
//


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
#include "flightProfessionals.h"


void AirlineMain(void* sock){
    char fromclient[2701];
    char toclient[2701];
    char flight_option;
    int rc2;
    //FILE *source;
    ssize_t n1;
    int j = 0;
    int true = 1;
    int ssock = (int) sock; /* slave server socket */
    char username[50];
    char password[50];
    char *hash_psw;
    char UID[5];
    char uidrestrict[50];
    char name[50];
    char airline_[50];
    char airlineRestrict[50];
    char f_source[50];
    char destination[50];
    char price[50];
    char total_seats[50];
    char open_seats[50];
    char date_depart[50];
    char date_arrive[50];
    char buffer[256];
    char local_cb[2701];
    char *point =toclient;
    char temp[256],temp2[256];
    char *ptemp;
    char *decrypt;
    char *encrypt;
    pthread_mutex_init(&uidcnfrmLock, NULL);
    pthread_mutex_init(&getfieldLock, NULL);
    pthread_mutex_init(&adduidLock, NULL);
    pthread_mutex_init(&callbackLock, NULL);
    pthread_mutex_init(&getUserLock, NULL);
    pthread_mutex_init(&userloginLock, NULL);


    /*Open Database*/
    rc2 = sqlite3_open("airline.db", &db2);
    if( rc2 ){
        fprintf(stderr, "Database failed to open: %s\n", sqlite3_errmsg(db2));
    }
    else
        fprintf(stderr, "Database opened successfully\n");
        int o = sqlite3_threadsafe();
        printf("sqlite_threadsafe flight: %d\n",o);
       char *user_pswd_hash = NULL;
    // create_table();
    // create_UID_store();
      //  add_col();
login:   getUserInfo((void*)ssock);
        strcpy(username,globalusr);
        strcpy(password,globalpassw);
        pthread_mutex_unlock(&getUserLock);
        hash_psw = hash_genn_md5(password, strlen(password));
        pthread_mutex_lock(&userloginLock);
        int a = login_flight_prof(username, hash_psw);
        pthread_mutex_unlock(&userloginLock);
        if (a == 2) {
        //user does not exist
            bzero(toclient,sizeof(toclient));
            printf("%s",wclientMsg);
            strcpy(toclient,usernotexistt);
            encrypt = Encryptt(toclient);
            n1=write(ssock,encrypt,strlen(encrypt));
            if (n1 <= 0){
                errexit("ERROR writing to socket\n");
            }
            bzero(fromclient,sizeof(fromclient));
            n1 = read(ssock,fromclient,sizeof(fromclient));
            decrypt=Decryptt(fromclient);
            bzero(fromclient,sizeof(fromclient));
            strcpy(fromclient,decrypt);
            if (n1 < 0) {
                errexit("ERROR reading from socket %s\n", strerror(errno));
            }
            if (fromclient[0] == '1') {
                goto login;
            }else goto exit;
        }else if (a == 0){
        //incorrect password
            bzero(toclient,sizeof(toclient));
            printf("%s",wclientMsg);
            strcpy(toclient,incorrectPswdMsgg);
            encrypt = Encryptt(toclient);
            n1=write(ssock,encrypt,strlen(encrypt));
            if (n1 <= 0){
                errexit("ERROR writing to socket\n");
            }
            bzero(fromclient,sizeof(fromclient));
            n1 = read(ssock,fromclient,sizeof(fromclient));
            decrypt=Decryptt(fromclient);
            bzero(fromclient,sizeof(fromclient));
            strcpy(fromclient,decrypt);
            if (n1 < 0) {
                errexit("ERROR reading from socket %s\n", strerror(errno));
            }
            if (fromclient[0] == '1') {
                goto login;
            }else goto exit;
        }else if (a == 1){
        //logged in successfully
            bzero(toclient,sizeof(toclient));
            printf("%s",wclientMsg);
            strcpy(toclient,loginSuccesMsg);
            encrypt = Encryptt(toclient);
            n1=write(ssock,encrypt,strlen(encrypt));
            if (n1 <= 0){
                errexit("ERROR writing to socket\n");
            }
        }
        /*Get uid from flight_users account to restrict user's
         ability to modify flights of other airlines*/
        pthread_mutex_lock(&getfieldLock);
        get_field("UID", "Flight_Users", "name",username);
        strcpy(uidrestrict, call_buff2);
        pthread_mutex_unlock(&getfieldLock);
        if(uidrestrict[strlen(uidrestrict) - 1] == '\n')
            uidrestrict[strlen(uidrestrict) - 1] = '\0';
    
    while (true){
    bzero(toclient,sizeof(toclient));
        printf("%s",wclientMsg);
    strcpy(toclient,flight_service_options);
        encrypt = Encryptt(toclient);
        n1=write(ssock,encrypt,strlen(encrypt));
    if (n1 <= 0){
        errexit("ERROR writing to socket\n");
    }
    bzero(fromclient,sizeof(fromclient));
    n1 = read(ssock,fromclient,sizeof(fromclient));
        decrypt=Decryptt(fromclient);
        bzero(fromclient,sizeof(fromclient));
        strcpy(fromclient,decrypt);
    if (n1 < 0) {
        errexit("ERROR reading from socket %s\n", strerror(errno));
    }
        flight_option = fromclient[0];
        switch (flight_option) {
            case '1':
                printf("Request from client to add flight\n");
                /*Get airline name from flight_users account to restrict
                 user's ability to modify flights of other airlines*/
                pthread_mutex_lock(&getfieldLock);
                get_field("airline", "UID_STORE", "UID", uidrestrict);
                strcpy(airlineRestrict, call_buff2);
                pthread_mutex_unlock(&getfieldLock);
                if(airlineRestrict[strlen(airlineRestrict) - 1] == '\n')
                    airlineRestrict[strlen(airlineRestrict) - 1] = '\0';
        add:     /*Request for flight name*/
                printf("%s",wclientMsg);
                bzero(toclient,sizeof(toclient));
                strcpy(toclient,flightNameQuery);
                encrypt = Encryptt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
                printf("%s",rclientMsg);
                bzero(fromclient,sizeof(fromclient));
                n1 = read(ssock,fromclient,sizeof(fromclient));
                decrypt=Decryptt(fromclient);
                bzero(fromclient,sizeof(fromclient));
                strcpy(fromclient,decrypt);
                if (n1 < 0) {
                    errexit("ERROR reading from socket %s\n", strerror(errno));
                }
                strcpy(name,fromclient);
                if ((strncmp(name, airlineRestrict,3)) != 0){
                    //incorrect name type for airline
                    char name_tag[3];
                    strncpy(name_tag, airlineRestrict,3);
                    printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    sprintf(toclient,"Flights of airline %s must begin with "\
                    "'%s' followed by flight number.\n",airlineRestrict,name_tag);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    goto add;
                }
                /*Request for flight source*/
                printf("%s",wclientMsg);
                bzero(toclient,sizeof(toclient));
                strcpy(toclient,flightSourceQuery);
                encrypt = Encryptt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
                printf("%s",rclientMsg);
                bzero(fromclient,sizeof(fromclient));
                n1 = read(ssock,fromclient,sizeof(fromclient));
                decrypt=Decryptt(fromclient);
                bzero(fromclient,sizeof(fromclient));
                strcpy(fromclient,decrypt);
                if (n1 < 0) {
                    errexit("ERROR reading from socket %s\n", strerror(errno));
                }
                strcpy(f_source,fromclient);
                /*Request for flight destination*/
                printf("%s",wclientMsg);
                bzero(toclient,sizeof(toclient));
                strcpy(toclient,flightDestinationQuery);
                encrypt = Encryptt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
                printf("%s",rclientMsg);
                bzero(fromclient,sizeof(fromclient));
                n1 = read(ssock,fromclient,sizeof(fromclient));
                decrypt=Decryptt(fromclient);
                bzero(fromclient,sizeof(fromclient));
                strcpy(fromclient,decrypt);
                if (n1 < 0) {
                    errexit("ERROR reading from socket %s\n", strerror(errno));
                }
                strcpy(destination,fromclient);
                /*Request for flight price*/
                printf("%s",wclientMsg);
                bzero(toclient,sizeof(toclient));
                strcpy(toclient,flightPriceQuery);
                encrypt = Encryptt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
                printf("%s",rclientMsg);
                bzero(fromclient,sizeof(fromclient));
                n1 = read(ssock,fromclient,sizeof(fromclient));
                decrypt=Decryptt(fromclient);
                bzero(fromclient,sizeof(fromclient));
                strcpy(fromclient,decrypt);
                if (n1 < 0) {
                    errexit("ERROR reading from socket %s\n", strerror(errno));
                }
                strcpy(price,fromclient);
                /*Request for total seats in flight*/
                printf("%s",wclientMsg);
                bzero(toclient,sizeof(toclient));
                strcpy(toclient,totalSeatQuery);
                encrypt = Encryptt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
                printf("%s",rclientMsg);
                bzero(fromclient,sizeof(fromclient));
                n1 = read(ssock,fromclient,sizeof(fromclient));
                decrypt=Decryptt(fromclient);
                bzero(fromclient,sizeof(fromclient));
                strcpy(fromclient,decrypt);
                if (n1 < 0) {
                    errexit("ERROR reading from socket %s\n", strerror(errno));
                }
                strcpy(total_seats,fromclient);
                /*Request for number of open seats in flight*/
                printf("%s",wclientMsg);
                bzero(toclient,sizeof(toclient));
                strcpy(toclient,openSeatsQuery);
                encrypt = Encryptt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
                printf("%s",rclientMsg);
                bzero(fromclient,sizeof(fromclient));
                n1 = read(ssock,fromclient,sizeof(fromclient));
                decrypt=Decryptt(fromclient);
                bzero(fromclient,sizeof(fromclient));
                strcpy(fromclient,decrypt);
                if (n1 < 0) {
                    errexit("ERROR reading from socket %s\n", strerror(errno));
                }
                strcpy(open_seats,fromclient);
                /*Request for total seats in flight*/
                printf("%s",wclientMsg);
                bzero(toclient,sizeof(toclient));
                strcpy(toclient,departDateQuery);
                encrypt = Encryptt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
                printf("%s",rclientMsg);
                bzero(fromclient,sizeof(fromclient));
                n1 = read(ssock,fromclient,sizeof(fromclient));
                decrypt=Decryptt(fromclient);
                bzero(fromclient,sizeof(fromclient));
                strcpy(fromclient,decrypt);
                if (n1 < 0) {
                    errexit("ERROR reading from socket %s\n", strerror(errno));
                }
                strcpy(date_depart,fromclient);
                /*Request for number of open seats in flight*/
                printf("writing to the client\n");
                bzero(toclient,sizeof(toclient));
                strcpy(toclient,arriveDateQuery);
                encrypt = Encryptt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
                printf("%s",rclientMsg);
                bzero(fromclient,sizeof(fromclient));
                n1 = read(ssock,fromclient,sizeof(fromclient));
                decrypt=Decryptt(fromclient);
                bzero(fromclient,sizeof(fromclient));
                strcpy(fromclient,decrypt);
                if (n1 < 0) {
                    errexit("ERROR reading from socket %s\n", strerror(errno));
                }
                strcpy(date_arrive,fromclient);
                bzero(buffer, sizeof(buffer));
                sprintf(buffer, "insert into DATA values ('%s', %s, '%s', '%s',"\
                "'%s', %s, %s, %s,'%s','%s');",name,uidrestrict,airlineRestrict,
                f_source,destination,price,total_seats,open_seats,date_depart,date_arrive);
               int p = add_flight(name,buffer);
                strcpy(local_cb, call_buff);
                pthread_mutex_unlock(&callbackLock);
                if (p == 1) {
                    printf("%s",wclientMsg);
                    point = &toclient[0];
                    bzero(temp, 256);
                    bzero(temp2, 256);
                    ptemp = flightAddSuccesMsg;
                    strcpy(temp, flightAddSuccesMsg);
                    memmove(temp2, ptemp+7, strlen(ptemp+7)+1);
                    memcpy(&temp[8], name, strlen(name));
                    memcpy(&temp[8+strlen(name)], temp2, strlen(temp2));
                    point += sprintf(point,"%s",temp);
                    bzero(temp, 256);
                    bzero(temp2, 256);
                    ptemp = flightSummaryMsg;
                    strcpy(temp, flightSummaryMsg);
                    memmove(temp2, ptemp+19, strlen(ptemp+19)+1);
                    memcpy(&temp[17],local_cb, strlen(local_cb));
                    memcpy(&temp[17+strlen(local_cb)], temp2, strlen(temp2));
                    point += sprintf(point,"%s",temp);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                }
                else
                {   printf("%s",wclientMsg);
                    point = &toclient[0];
                    bzero(temp, 256);
                    bzero(temp2, 256);
                    ptemp = addflightFailMsg;
                    strcpy(temp, addflightFailMsg);
                    memmove(temp2, ptemp+6, strlen(ptemp+6)+1);
                    memcpy(&temp[7], name, strlen(name));
                    memcpy(&temp[7+strlen(name)], temp2, strlen(temp2));
                    point += sprintf(point,"%s",temp);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                    }
                }
                printf("%s",rclientMsg);
                bzero(fromclient,sizeof(fromclient));
                n1 = read(ssock,fromclient,sizeof(fromclient));
                decrypt=Decryptt(fromclient);
                bzero(fromclient,sizeof(fromclient));
                strcpy(fromclient,decrypt);
                if (n1 < 0) {
                    errexit("ERROR reading from socket %s\n", strerror(errno));
                }
                if ((fromclient[0] == 'y')||(fromclient[0] == 'Y')) {
                    goto add;
                }
                else break;
            case '2':
        search: printf("Request from client to view a flight\n");
                /*Request for  name of flight*/
                printf("%s",wclientMsg);
                bzero(toclient,sizeof(toclient));
                strcpy(toclient,flightNameQuery);
                encrypt = Encryptt(toclient);
                n1=write(ssock,encrypt,strlen(encrypt));
                if (n1 <= 0){
                    errexit("ERROR writing to socket\n");
                }
                printf("%s",rclientMsg);
                bzero(fromclient,sizeof(fromclient));
                n1 = read(ssock,fromclient,sizeof(fromclient));
                decrypt=Decryptt(fromclient);
                bzero(fromclient,sizeof(fromclient));
                strcpy(fromclient,decrypt);
                if (n1 < 0) {
                    errexit("ERROR reading from socket %s\n", strerror(errno));
                }
                strcpy(name,fromclient);
                int i = 0;
                bzero(local_cb, sizeof(local_cb));
                i = view_flight(name);
                strcpy(local_cb, call_buff);
                pthread_mutex_unlock(&callbackLock);
                if(i != 1){
                    printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    sprintf(toclient,"%s", flightnotExistMsg);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                }
                else if (i == 1){
                    printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    sprintf(toclient, "\nSearch Result:\n\n%s\n"\
                    "Enter y to search more flights, n to exit",local_cb);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                }
                printf("%s",rclientMsg);
                bzero(fromclient,sizeof(fromclient));
                n1 = read(ssock,fromclient,sizeof(fromclient));
                decrypt=Decryptt(fromclient);
                bzero(fromclient,sizeof(fromclient));
                strcpy(fromclient,decrypt);
                if (n1 < 0) {
                    errexit("ERROR reading from socket %s\n", strerror(errno));
                }
                if ((fromclient[0] == 'y')||(fromclient[0] == 'Y')) {
                    goto search;
                }
                break;
            case '3':
                    printf("Request from client to modify flight\n");
                    /*Get airline name from flight_users account to restrict
                     user's ability to modify flights of other airlines*/
                    pthread_mutex_lock(&getfieldLock);
                    get_field("airline", "UID_STORE", "UID", uidrestrict);
                    bzero(airlineRestrict, sizeof(airlineRestrict));
                    strcpy(airlineRestrict, call_buff2);
                    pthread_mutex_unlock(&getfieldLock);
                    if(airlineRestrict[strlen(airlineRestrict) - 1] == '\n')
                    airlineRestrict[strlen(airlineRestrict) - 1] = '\0';
            modify: /*Request for  name of flight*/
                    printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    strcpy(toclient,flightNameQuery);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    strcpy(name,fromclient);
                    /*Confirm that flight exists in database*/
                    int o = 0;
                    o = view_flight(name);
                    pthread_mutex_unlock(&callbackLock);
                    if(o != 1){
                        printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    strcpy(toclient,flightnotExistMsg2);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                        goto modify;
                    }
                    /*Check to make sure user is authorized to modify flight*/
                    pthread_mutex_lock(&getfieldLock);
                    get_field("airline","data","name",name);
                    strcpy(airline_, call_buff2);
                    pthread_mutex_unlock(&getfieldLock);
                    if(airline_[strlen(airline_) - 1] == '\n')
                        airline_[strlen(airline_) - 1] = '\0';
                    if ((strcmp(airlineRestrict, airline_)) != 0){
                    //not authorized to modify flight
                        printf("%s",wclientMsg);
                        bzero(toclient,sizeof(toclient));
                        sprintf(toclient,"%s",modifyUnauthorizedMsg);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                        printf("%s",rclientMsg);
                        bzero(fromclient,sizeof(fromclient));
                        n1 = read(ssock,fromclient,sizeof(fromclient));
                        decrypt=Decryptt(fromclient);
                        bzero(fromclient,sizeof(fromclient));
                        strcpy(fromclient,decrypt);
                        if (n1 < 0) {
                            errexit("ERROR reading from socket %s\n", strerror(errno));
                        }
                        if (fromclient[0] == '1') {
                            goto modify;
                        }else break;
                    }
            more:   printf("%s",wclientMsg);
                    point = &toclient[0];
                    point += sprintf(point,"\nRequest to modify flight %s with airline ID %s.\n\n",name,uidrestrict);
                    point += sprintf(point,"%s\n",modifyAllMsg);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    if (fromclient[0] == '1') {
                        j = 1;
                    }
                    int a = 1;
                    bzero(buffer, sizeof(buffer));
                    if((j == 1)||((fromclient[0] == '2')&&(a == 1))){
                    /*Request for flight source*/
                    a =0;
                        printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    strcpy(toclient,flightSourceQuery);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    strcpy(f_source,fromclient);
                    sprintf(buffer, "UPDATE DATA set SOURCE = '%s' where name = '%s'",f_source, name);
                    }
                    if((j == 1)||((fromclient[0] == '3')&&(a == 1))){
                    /*Request for flight destination*/
                    a = 0;
                        printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    strcpy(toclient,flightDestinationQuery);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    strcpy(destination,fromclient);
                    sprintf(buffer, "UPDATE DATA set DESTINATION = '%s' where name = '%s'",destination, name);
                    }
                    if((j == 1)||((fromclient[0] == '4')&&(a == 1))){
                    /*Request for flight price*/
                    a = 0;
                        printf("%s",wclientMsg);
                    bzero(toclient,2001);
                    strcpy(toclient,flightPriceQuery);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                        printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    strcpy(price,fromclient);
                    sprintf(buffer, "UPDATE DATA set PRICE = %s where name = '%s'",price, name);
                    }
                    if((j == 1)||((fromclient[0] == '5')&&(a == 1))){
                    /*Request for number of open seats in flight*/
                    a = 0;
                        printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    strcpy(toclient,openSeatsQuery);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                        printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    strcpy(open_seats,fromclient);
                    sprintf(buffer, "UPDATE DATA set OPEN_SEATS = %s where name = '%s'",open_seats, name);
                    }
                    if((j == 1)||((fromclient[0] == '6')&&(a == 1))){
                    /*Request for Departure date/time*/
                    a = 0;
                        printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    strcpy(toclient,departDateQuery);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                        printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    strcpy(date_depart,fromclient);
                    sprintf(buffer, "UPDATE DATA set DATE_DEPATURE = '%s' where name = '%s'",date_depart, name);
                    }
                    if((j == 1)||((fromclient[0] == '7')&&(a == 1))){
                    /*Request for Date/Time of Arrival*/
                    a = 0;
                        printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    strcpy(toclient,arriveDateQuery);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                        printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    strcpy(date_arrive,fromclient);
                    sprintf(buffer, "UPDATE DATA set DATE_ARRIVAL = '%s' where name = '%s'",date_arrive, name);
                    }
                    if (j == 1) {
                        bzero(buffer, sizeof(buffer));
                        sprintf(buffer, "UPDATE DATA set SOURCE = '%s',DESTINATION = '%s',"\
                        "PRICE = %s, OPEN_SEATS = %s,DATE_DEPATURE = '%s',DATE_ARRIVAL = '%s' where name = '%s'",
                        f_source, destination, price, open_seats,date_depart,date_arrive, name);
                        j = 0;
                    }
                    bzero(local_cb, sizeof(local_cb));
                    int y = modify_flight(name,buffer);
                    strcpy(local_cb, call_buff);
                    pthread_mutex_unlock(&callbackLock);
                    if (y != 1) {
                        printf("%s",wclientMsg);
                        bzero(toclient,sizeof(toclient));
                        sprintf(toclient,"%s",modifyflightErrMsg);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                    }else{
                        printf("%s",wclientMsg);
                        bzero(toclient,sizeof(toclient));
                        sprintf(toclient,"Updated Flight Details for %s:\n\n%s\n"\
                                "Enter y to modify more fields, n to exit",name,local_cb);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                    }
                    printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    if ((fromclient[0] == 'y')||(fromclient[0] == 'Y')) {
                        goto more;
                    }
                    break;
            case '4':
                    printf("Request from client to delete flight\n");
                    /*Get airline name from flight_users account to restrict
                     user's ability to modify flights of other airlines*/
                    pthread_mutex_lock(&getfieldLock);
                    get_field("airline", "UID_STORE", "UID", uidrestrict);
                    strcpy(airlineRestrict, call_buff2);
                    pthread_mutex_unlock(&getfieldLock);
                    if(airlineRestrict[strlen(airlineRestrict) - 1] == '\n')
                    airlineRestrict[strlen(airlineRestrict) - 1] = '\0';
                    /*Request for  name of flight*/
            delete: printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    strcpy(toclient,flightNameQuery);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    printf("reading from the client\n");
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    strcpy(name,fromclient);
                    int d = 0;
                    d = view_flight(name);
                    pthread_mutex_unlock(&callbackLock);
                    if(d!= 1){
                        printf("%s",wclientMsg);
                        bzero(toclient,sizeof(toclient));
                        strcpy(toclient,flightnotExistMsg2);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                        goto delete;
                    }
                    /*Check to make sure user is authorized to delete flight*/
                    pthread_mutex_lock(&getfieldLock);
                    get_field("airline", "data", "name", name);
                    strcpy(airline_, call_buff2);
                    pthread_mutex_unlock(&getfieldLock);
                    if(airline_[strlen(airline_) - 1] == '\n')
                    airline_[strlen(airline_) - 1] = '\0';
                    if ((strcmp(airlineRestrict,airline_)) != 0){
                    //unauthorized user
                        printf("%s",wclientMsg);
                        bzero(toclient,sizeof(toclient));
                        sprintf(toclient,"%s",deleteunauthorizedMsg);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                        printf("%s",rclientMsg);
                        bzero(fromclient,sizeof(fromclient));
                        n1 = read(ssock,fromclient,sizeof(fromclient));
                        decrypt=Decryptt(fromclient);
                        bzero(fromclient,sizeof(fromclient));
                        strcpy(fromclient,decrypt);
                        if (n1 < 0) {
                            errexit("ERROR reading from socket %s\n", strerror(errno));
                        }
                        if (fromclient[0] == '1') {
                            goto delete;
                        }else break;
                    }
                    d=0;
                    d = delete_("DATA","NAME",name);
                    if (d != 1){
                        printf("%s",wclientMsg);
                        bzero(toclient,sizeof(toclient));
                        sprintf(toclient,"%s",deleteflightErrMsg);
                    }else{
                        printf("writing to the client\n");
                        point = &toclient[0];
                        bzero(temp, 256);
                        bzero(temp2, 256);
                        ptemp = flightdeleteSuccesSmSG;
                        strcpy(temp, ptemp);
                        memmove(temp2, ptemp+6, strlen(ptemp+6)+1);
                        memcpy(&temp[7], name, strlen(name));
                        memcpy(&temp[7+strlen(name)], temp2, strlen(temp2));
                        point += sprintf(point,"%s",temp);
                    }
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    if ((fromclient[0] == 'y')||(fromclient[0] == 'Y')) {
                        goto delete;
                    }
                    break;
            case '5':
                    printf("Request from client to view all available flights \n");
                    bzero(local_cb, sizeof(local_cb));
                    j = view_all_flights();
                    strcpy(local_cb, call_buff);
                    pthread_mutex_unlock(&callbackLock);
                    if( j != 1){
                        printf("%s",wclientMsg);
                        bzero(toclient,sizeof(toclient));
                        strcpy(toclient,noflightsMsg);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                    }else{
                        printf("%s",wclientMsg);
                        point = &toclient[0];
                        point += sprintf(point,"%s",flightsearchResultMsg);
                        point += sprintf(point, "%s",local_cb);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                    }
                    break;
            case '6':
    search_airline: printf("Request from client to view all flights of an airline \n");
                    /*Request for  name of airline*/
                    printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    strcpy(toclient,airlineNameQuery);
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    strcpy(airline_,fromclient);
                    /* Create select SQL statement */
                    bzero(local_cb, sizeof(local_cb));
                    int m = view_airline(airline_);
                    strcpy(local_cb, call_buff);
                    pthread_mutex_unlock(&callbackLock);
                    if(m != 1){
                        printf("%s",wclientMsg);
                        point = &toclient[0];
                        point += sprintf(point,"%s",airlinenotExistMsg);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                    }else{
                        printf("%s",wclientMsg);
                        point = &toclient[0];
                        point += sprintf(point,"\nSearch Result: "\
                        "all for airline %s\n\n%s\n",airline_,local_cb);
                        point += sprintf(point,"%s",searchMoreMsg);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                    }
                    printf("%s",rclientMsg);
                    bzero(fromclient,sizeof(fromclient));
                    n1 = read(ssock,fromclient,sizeof(fromclient));
                    decrypt=Decryptt(fromclient);
                    bzero(fromclient,sizeof(fromclient));
                    strcpy(fromclient,decrypt);
                    if (n1 < 0) {
                        errexit("ERROR reading from socket %s\n", strerror(errno));
                    }
                    if ((fromclient[0] == 'y')||(fromclient[0] == 'Y')) {
                        goto search_airline;
                    }
                    break;
            case '7':
 delete_airline:    printf("Request from client to delete an airline\n");
                    /*If sys admin, delete airline else break*/
                    if ((strcmp(username, "System_Admin")) == 0) {
                        /*Request for airline ID*/
                        printf("%s",wclientMsg);
                        bzero(toclient,sizeof(toclient));
                        strcpy(toclient,airlineIDQuery);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                        printf("%s",rclientMsg);
                        bzero(fromclient,sizeof(fromclient));
                        n1 = read(ssock,fromclient,sizeof(fromclient));
                        decrypt=Decryptt(fromclient);
                        bzero(fromclient,sizeof(fromclient));
                        strcpy(fromclient,decrypt);
                        if (n1 < 0) {
                            errexit("ERROR reading from socket %s\n", strerror(errno));
                        }
                        strcpy(UID,fromclient);
                        pthread_mutex_lock(&getfieldLock);
                        int x = get_field("airline", "UID_STORE", "UID", UID);
                        sprintf(airline_,"%s", call_buff2);
                        pthread_mutex_unlock(&getfieldLock);
                        if(airline_[strlen(airline_) - 1] == '\n')
                           airline_[strlen(airline_) - 1] = '\0';
                        if (x == 0) {
                            printf("%s",wclientMsg);
                            bzero(toclient,sizeof(toclient));
                            strcpy(toclient, "\nUID does not exist on our server\n");
                            encrypt = Encryptt(toclient);
                            n1=write(ssock,encrypt,strlen(encrypt));
                            if (n1 <= 0){
                                errexit("ERROR writing to socket\n");
                            }
                            break;
                        }
                        /*Confirm delete*/
                        printf("%s",wclientMsg);
                        bzero(toclient,sizeof(toclient));
                        sprintf(toclient, "\nDelete all flights from airline %s?\n"\
                        "<<:: 1. Confirm Delete ::>> <<:: 2. Cancel ::>>",airline_);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                        printf("%s",rclientMsg);
                        bzero(fromclient,sizeof(fromclient));
                        n1 = read(ssock,fromclient,sizeof(fromclient));
                        decrypt=Decryptt(fromclient);
                        bzero(fromclient,sizeof(fromclient));
                        strcpy(fromclient,decrypt);
                        if (n1 < 0) {
                            errexit("ERROR reading from socket %s\n", strerror(errno));
                        }
                        if (fromclient[0] == '1'){
                        /*delete*/
                            int d = delete_("DATA","UID",UID);
                            printf("%s",wclientMsg);
                            bzero(toclient,sizeof(toclient));
                            if (d == 1) {
                            /*success*/
                                d = delete_("UID_STORE","UID",UID);
                                if (d == 1) {
                                /*success! airline and uid deleted*/
                                    sprintf(toclient,"%s",airlinedeletesuccessMsg);
                                }/*airline deleted but not uid*/
                                else sprintf(toclient,"%s",airlineUidnotDeleted);
                            }/*airline delete failed*/
                            else sprintf(toclient,"%s",deleteairlineErrMsg);
                            encrypt = Encryptt(toclient);
                            n1=write(ssock,encrypt,strlen(encrypt));
                            if (n1 <= 0){
                                errexit("ERROR writing to socket\n");
                            }
                        }else break;
                    }else{
                        printf("%s",wclientMsg);
                        bzero(toclient,sizeof(toclient));
                        sprintf(toclient, "%s",unauthorizedUser);
                        encrypt = Encryptt(toclient);
                        n1=write(ssock,encrypt,strlen(encrypt));
                        if (n1 <= 0){
                            errexit("ERROR writing to socket\n");
                        }
                    }
                    break;
            case '8':
            exit:   printf("%s",wclientMsg);
                    bzero(toclient,sizeof(toclient));
                    strcpy(toclient,"Goodbye!");
                    encrypt = Encryptt(toclient);
                    n1=write(ssock,encrypt,strlen(encrypt));
                    if (n1 <= 0){
                        errexit("ERROR writing to socket\n");
                    }
                    true = 0;
                    break;
            default:
                    errexit("Invalid request\n");
                    break;
        }
    }
}


