#include "rtc.h"
#include <stdbool.h>
#include <string.h>
#include <isr.h>
#include <string.h>
#include <io.h>


time_t global_time;
bool bcd;
unsigned char read_register(unsigned char reg)
{
    outb(0x70, reg);
    return inb(0x71);
}

void write_register(unsigned char reg, unsigned char value)
{
    outb(0x70, reg);
    outb(0x71, value);
}

unsigned char bcd2bin(unsigned char bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void gettime(time_t *time)
{
    memcpy(time, &global_time, sizeof(time_t));
}

void rtc_handler(context_t* context)
{
    if(read_register(0x0C) & 0x10){
        if(bcd){
            global_time.second = bcd2bin(read_register(0x00));
            global_time.minute = bcd2bin(read_register(0x02));
            global_time.hour   = bcd2bin(read_register(0x04));
            global_time.month  = bcd2bin(read_register(0x08));
            global_time.year   = bcd2bin(read_register(0x09));
            global_time.day_of_week  = bcd2bin(read_register(0x06));
            global_time.day_of_month = bcd2bin(read_register(0x07));
        }else {
            global_time.second = read_register(0x00);
            global_time.minute = read_register(0x02);
            global_time.hour   = read_register(0x04);
            global_time.month  = read_register(0x08);
            global_time.year   = read_register(0x09);
            global_time.day_of_week  = read_register(0x06);
            global_time.day_of_month = read_register(0x07);
        }
    }
}

void rtc_install(void)
{
    unsigned char status;
    status = read_register(0x0B);
    status |=  0x02;             // 24 hour clock
    status |=  0x10;             // update ended interrupts
    status &= ~0x20;             // no alarm interrupts
    status &= ~0x40;             // no periodic interrupt
    bcd     =  !(status & 0x04); // check if data type is BCD
    write_register(0x0B, status);
    read_register(0x0C);
    register_interrupt_handler(8, &rtc_handler);
}