#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

void* memset(void*, int, size_t);
size_t strlen(const char* str);
void strrev(char* str);
char* strcat(char *dest, const char *src);
int strcmp(const char *l, const char *r);
//char* strcatbyte(char dest[], char source);
char* append(char* s, char c);
char* strcpy(char* destination, const char* source);
void atoi(char *str, int* a);

#endif