#include <cpu_speed.h>
#include <timer.h>

int discoverspeed(){
    uint32_t hsec = getmsecfromboot();
    uint32_t clock = 0;
    while(getmsecfromboot() - hsec != 1000){
        //15 cycles per each
        cpu_cycle();
        clock++;
    }
    return (clock * 15) / 10000000;
}