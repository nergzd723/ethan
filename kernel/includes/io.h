#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>

void cpu_cycle();

int discoverspeed();

static inline void outb (unsigned short _port, unsigned char _data) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

//clear interrupts
static inline void cli(void){
    asm volatile( "cli" );
}

static inline void halt(void){
    asm volatile( "hlt" );
}

static inline void io_wait(void) {
    /* TODO: This is probably fragile. */
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}
#endif