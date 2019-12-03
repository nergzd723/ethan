#include <frame_buffer.h>
#include <timer.h>
#include <drawing.h>
void updlat(uint32_t time){
    drawlatency = time;
    return;
}
uint32_t drawlatency = 0;

void draw(uint32_t FB_COL, uint32_t FB_COLTO)
{
    
}