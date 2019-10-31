#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

#define TIMER_FREQUENCY 1000
#define MILLISECOND 1000

void init_timer(uint32_t frequency);
int getsecfromboot();
void wait(int second);
void waitm(int msec);
uint32_t getmsecfromboot();
void sleep(uint32_t microsecond);
#endif