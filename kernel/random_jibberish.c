#include <timer.h>
#include <random.h>
#include <frame_buffer.h>
#include <io.h>

void dojibberish(){
    asm volatile("sti");
    clear_screen();
    while(1){
        fb_write_cell(rand()%FB_CELLS, rand(), FB_GREEN, FB_BLACK);
        move_cursor_to_pos(rand()%FB_CELLS);
    }
}