#include <string.h>

char* strcat(char dest[], const char* source)
{
    int i = 0;
    int len = strlen(dest);
    while(source[i]){
    dest[len+i] = source[i];
    }
    return dest;
}

char* strcatbyte(char dest[], char source)
{
    int len = strlen(dest);
    dest[len+1] = source;
    return dest;
}