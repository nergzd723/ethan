#include <random.h>
#include <frame_buffer.h>
#include <timer.h>

void staralligns()
{
    asm volatile("sti");
    clear_screen();
    int a, starnumber;
    while (1)
    {
        a = rand();
        starnumber = a % FB_CELLS;
        for (int i = 0; i<starnumber; i++){
            fb_write_cell(a%FB_CELLS, '.', FB_WHITE, FB_BLACK); 
        }
        waitm(70);
        clear_screen();
    }

}