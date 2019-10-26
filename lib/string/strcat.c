#include <string.h>

char *
strcat(char *dest, const char *src)
{
    size_t i,j;
    for (i = 0; dest[i] != '\0'; i++)
        ;
    for (j = 0; src[j] != '\0'; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
}

char* strcatbyte(char dest[], char source)
{
    int len = strlen(dest);
    dest[len+1] = source;
    return dest;
}

//append by char
char* append(char* s, char c) {
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
    return s;
}