#include <timer.h>
#include <random.h>
#include <frame_buffer.h>

void dojibberish(){
    asm volatile("sti");
    clear_screen();
    while(1){
        fb_write_cell(rand()%FB_CELLS, (char)inttostr(rand()%128), FB_GREEN, FB_BLACK);
    }
}