#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>
extern void scall();
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

//get cpu uptime in ticks
static inline uint64_t rdtsc()
{
    uint64_t ret;
    asm volatile ( "rdtsc" : "=A"(ret) );
    return ret;
}

static inline void write_eax(int eaxyz)
{
    __asm__ __volatile__("mov %%eax, %0" : "=a"(eaxyz));
}

static inline void outw (uint16_t port, uint16_t val)
{
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint16_t inw(uint16_t port){
    uint16_t ret;
    asm volatile ( "inw %1, %0" : "=a"(ret) : "Nd"(port) );
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