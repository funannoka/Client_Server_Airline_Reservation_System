#if defined(__APPLE__)
#define COMMON_DIGEST_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>
//#include <CommonCrypto/CommonCryptor.h>
#define CCCryptor
#define SHA1 CC_SHA1
#define SHA512 CC_SHA1
#define MD5 CC_MD5
#define CCMode kCCModeCBC
#define CCAlgorithm kCCAlgorithmAES128
#else
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/md5.h>
#endif



extern char db_call_result[1500];
extern int pass_match=0;
sqlite3 *db;
sqlite3_stmt *stmt;
int rc;
char* query;

extern int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    pass_match=0;
    //   fprintf(stdout, "%s: ", (const char*)data);
    for(i=0; i<argc; i++){
        //  printf("%s = %s\n",(const char*) data,argv[i]);
        if(strcmp((const char*) data,argv[i])==0){
            pass_match=1;
        }
    }
   // printf("\n");
    return 0;
}


extern int callback_view_users(void *data, int argc, char **argv, char **azColName){
    int i;
    // fprintf(stderr, "%s: ", (const char*)data);
    for(i=0; i<argc; i++){
        strcat(db_call_result,argv[i]);
        strcat(db_call_result,"\n");
    }
    return 0;
}

extern int execute_command_db(){
    
    /*
     * execute commands that do not return results
     */
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
        strcpy(db_call_result,"Error !!!\n");
    }
    else{
        strcpy(db_call_result,"Retrived/Modified Successfully\n");
    }
    sqlite3_finalize(stmt);
    free(query);
    return 0;
}
