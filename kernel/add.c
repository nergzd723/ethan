#include <stdin.h>
#include <frame_buffer.h>
#include <string.h>
#include <user.h>

void add(){
    initgetchar();
    char n1 = getchar();
    initgetchar();
    char n2 = getchar();
    printf("Done calculating! N1 ");
    printf(" N2 ");
    printf("\n");
    printf(inttostr(n2+n1));
    printf("\n");
}
