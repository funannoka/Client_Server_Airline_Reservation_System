//
//  generateDigest.h
//  tcpserver_airline_project
//
//  Created by Ifunanya Nnoka on 11/29/16.
//  Copyright © 2016 Ifunanya Nnoka. All rights reserved.
//

#ifndef generateDigest_h
#define generateDigest_h
#if defined(__APPLE__)
#define COMMON_DIGEST_FOR_OPENSSL
#define COMMON_CRYPTOR_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>
#define SHA1 CC_SHA1
#define MD5 CC_MD5
#else
#include <openssl/md5.h>
#endif
char *gen_hash_md5(const char *field, int len){
    int i;
    MD5_CTX p;
    unsigned char digest[MD5_DIGEST_LENGTH];
    char *hash = (char*)malloc(33);
    
    MD5_Init(&p);
    while (len > 0) {
        if (len > 512) {
            MD5_Update(&p, field, 512);
        } else {
            MD5_Update(&p, field, len);
        }
        len -= 512;
        field += 512;
    }
    MD5_Final(digest, &p);
    for (i = 0; i < sizeof(digest); ++i) {
        snprintf(&(hash[i*2]), MD5_DIGEST_LENGTH*2,"%02x",(unsigned int)digest[i]);
    }
    return hash;
}

#endif /* generateDigest_h */
