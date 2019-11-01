#include <random.h>
#include <frame_buffer.h>
#include <timer.h>

void staralligns()
{
    asm volatile("sti");
    clear_screen();
    while (1)
    {
        int starnumber = rand() % FB_CELLS;
        for (int i = 0; i<starnumber; i++){
            fb_write_cell(rand()%FB_CELLS, '.', FB_WHITE, FB_BLACK); 
        }
        waitm(70);
    }

}