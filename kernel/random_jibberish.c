#include <timer.h>
#include <random.h>
#include <frame_buffer.h>

void dojibberish(){
    asm volatile("sti");
    clear_screen();
    int a;
    while(1){
        int a = rand();
        fb_write_cell(a%FB_CELLS, rand(), FB_GREEN, FB_BLACK);
        move_cursor_to_pos(a%FB_CELLS);
    }
}