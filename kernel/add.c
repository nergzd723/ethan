#include <stdin.h>
#include <frame_buffer.h>
#include <string.h>

void add(){
    initgetchar();
    char n1 = getchar();
    initgetchar();
    char n2 = getchar();
    printf("Done calculating! N1 ");
    printf(n1);
    printf(" N2 ");
    printf(n2);
    printf("\n");
    printf(inttostr(n2+n1));
    printf("\n");
}