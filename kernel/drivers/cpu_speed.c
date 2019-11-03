#include <cpu_speed.h>
#include <timer.h>
#include <io.h>

int discoverspeed(){
    uint64_t rtdsc1 = rdtsc();
    waitm(10);
    uint64_t rtdsc2 = rtdsc();
    uint8_t clock = (rtdsc2-rtdsc1)/100/1000;
    return clock;
}