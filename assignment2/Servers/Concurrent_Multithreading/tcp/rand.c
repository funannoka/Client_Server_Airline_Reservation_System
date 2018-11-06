//
//  random.c
//  tcp_client
//
//  Created by Ifunanya Nnoka on 10/16/16.
//  Copyright © 2016 Ifunanya Nnoka. All rights reserved.
//

#include "random.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LEN 16//128bits
int main()
{
    unsigned char *key = (unsigned char *) malloc(sizeof(unsigned char) *LEN);
    FILE* random = fopen("/dev/urandom","r");
    fread(key,sizeof(unsigned char) *LEN, 1, random);
    printf("Random number is %s \n",key);
    fclose(random);
    return 0;
}