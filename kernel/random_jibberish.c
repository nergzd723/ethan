#include <timer.h>
#include <random.h>
#include <frame_buffer.h>

void dojibberish(int sec){
    clear_screen();
    int i = getsecfromboot();
    while(getsecfromboot() - i != sec){
        printf(inttostr(rand));
    }
    clear_screen();
    reset_cursor();
    return;
}