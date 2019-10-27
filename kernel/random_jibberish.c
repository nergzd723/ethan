#include <timer.h>
#include <random.h>
#include <frame_buffer.h>

void dojibberish(){
    clear_screen();
    while(1){
        printf(inttostr(rand()));
    }
}