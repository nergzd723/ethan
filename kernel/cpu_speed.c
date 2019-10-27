#include <cpu_speed.h>
#include <timer.h>

int discoverspeed(){
    uint32_t hsec = getmsecfromboot();
    uint32_t clock = 0;
    while(getmsecfromboot() - hsec != 100){
        //4 cycles per each
        cpu_cycle();
        clock++;
    }
    return (clock * 4) / 1000000;
}