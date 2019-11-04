#include <cpu_speed.h>
#include <timer.h>
#include <io.h>
#include <sysinit.h>
#include <stdint.h>
uint8_t clock;
int discoverspeed(){
    if (__init_done){
        return clock;
    }
    uint64_t rdtsc1 = rdtsc();
    waitm(10);
    uint64_t rdtsc2 = rdtsc();
    clock = (rdtsc2-rdtsc1)/100/1000;
    return clock;
}