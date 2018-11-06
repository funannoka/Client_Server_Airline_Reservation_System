#include <stdio.h>
#include <string.h>

char * Encrypt(char *string)
{
    char *linee =  (char *)malloc(sizeof(char) * 50);
    
    int n=strlen(string);
    int i=0;
    for(i=0;i<n;i++){
        string[i] = string[i]+25;
    }
    //	printf("%s\n",string);
    strcpy(linee, string);
    return linee;
    
}

char * Decrypt (char *string)
{
    char *linee =  (char *)malloc(sizeof(char) * 2701);
    int n=strlen(string);
    int i=0;
    for(i=0;i<n;i++){
        string[i] = string[i]-25;
    }
    strcpy(linee, string);
    return linee;
}

