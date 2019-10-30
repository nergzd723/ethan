#include <stdin.h>
#include <frame_buffer.h>
#include <string.h>

void add(){
    initinput();
    char* n1 = input();
    char* n2 = input();
    int i1, i2;
    atoi(n1, i1);
    atoi(n2, i2);
    printf("Done calculating! N1 ");
    printf(n1);
    printf(" N2 ");
    printf(n2);
    printf("\n");
    printf(inttostr(i2+i1));
    printf("\n");
}