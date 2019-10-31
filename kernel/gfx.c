#include <rmonitor.h>
#include <string.h>
#include <timer.h>
#include <frame_buffer.h>

//test
void gfx_test()
{
    asm volatile("sti");
    int y;
    regs16_t regs;
     
    // switch to 320x200x256 graphics mode
    regs.ax = 0x0013;
    int32(0x10, &regs);
     
    // full screen with blue color (1)
    memset((char *)0xA0000, 1, (320*200));
     
    // draw horizontal line from 100,80 to 100,240 in multiple colors
    for(y = 0; y < 200; y++)
        memset((char *)0xA0000 + (y*320+80), y, 160);
    wait(3);
    regs.ax = 0x0003;
    int32(0x10, &regs);
    clear_screen();
}