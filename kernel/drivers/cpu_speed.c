#include <cpu_speed.h>
#include <timer.h>
#include <io.h>

int discoverspeed(){
    uint64_t rdtsc1 = rdtsc();
    waitm(10);
    uint64_t rdtsc2 = rdtsc();
    uint8_t clock = (rdtsc2-rdtsc1)/100/1000;
    return clock;
}