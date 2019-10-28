#include <timer.h>
#include <isr.h>
#include <io.h>
#include <logger.h>
#include <stdint.h>

uint32_t ticks = 0;

uint32_t read_timer(void) {
   return ticks;
}

// Returns the number of ticks that have elapsed in the interval
uint32_t elapsed_time(uint32_t start, uint32_t end) {
   if (end >= start) {
      // No overflow
      return end - start;
   } else {
      // Handle overflow
      return UINT64_MAX - start + end;
   }
}

int sec = 0;
uint32_t msec = 0;

static void timer_callback(context_t* context)
{
   ticks++;
   if (ticks % MILLISECOND == 0) {
      msec++;
   }
   if (ticks % TIMER_FREQUENCY == 0){
      sec++;
      ticks = 0;
   }
}

uint32_t getmsecfromboot(){
   return msec;
}

int getsecfromboot(){
   return sec;
}

//wait millisecond
void waitm(int ms){
   int olds = msec;
   while ((msec-olds) != ms){}
   return;
}

void wait(int second){
   int oldsec = sec;
   while ((sec-oldsec) != second){}
   return;
   
}
void init_timer(uint32_t frequency)
{
   // Firstly, register our timer callback.
   register_interrupt_handler(32, &timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   uint32_t divisor = 1193180 / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}