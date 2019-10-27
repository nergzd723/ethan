#include <io.h>
#include <timer.h>

int discoverspeed(){
    uint32_t hsec = getmsfromboot();
    uint32_t clock = 0;
    while(getmsfromboot() - hsec != 100){
        //4 cycles per each
        cpucycle();
        clock++;
    }
    return (clock * 4) / 1000000;
}