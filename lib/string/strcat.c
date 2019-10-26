#include <string.h>

char* strcat(char *dest, const char *src)
{
    char *rdest = dest;

    while (*dest)
      dest++;
    while (*dest++ = *src++)
      ;
    return rdest;
}

char* strcatbyte(char dest[], char source)
{
    int len = strlen(dest);
    dest[len+1] = source;
    return dest;
}