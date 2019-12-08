#include <timer.h>
#include <random.h>
#include <frame_buffer.h>
#include <io.h>

void dojibberish(){
    asm volatile("sti");
    clear_screen();
    while(1){
        for(int cell = 0; cell<FB_CELLS; cell++){
            fb_write_cell(cell, rand(), FB_GREEN, FB_BLACK);
        }
    }
}