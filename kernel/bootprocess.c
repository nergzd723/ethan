#include <idt.h>
#include <pic.h>
#include <io.h>
#include <logger.h>
#include <gdt.h>
#include <timer.h>
#include <keyboard.h>
#include <memory.h>
#include <frame_buffer.h>
#include <isr.h>
#include <defines.h>
#include <cpu_speed.h>
#include <random.h>
#include <boot.h>
#include <rtc.h>
#include <sb16.h>
#include <v8086m.h>
#include <smbios.h>

void boot_stage2(){
   printf("Enabling hardware interrupts...\n");
   __asm__ volatile("sti");
   printf("OK\n");
   printf("Testing timer...\n");
   waitm(30);
   printf("SoundBlaster16...\n");
   sb16_init();
   printf("OK\n");
   clear_screen();
   printf("Ethanium booted! Got ");
   printf(inttostr(upper_memory()+lower_memory()));
   printf("K mem total, ");
   printf(inttostr(lower_memory()));
   printf("K lowmem, ");
   printf(inttostr(upper_memory()));
   printf("K USABLE extended memory\n");
   if (memorycount() > 65535){
       printf("It seems that you have more than 64M of RAM. Ethanium may not work properly\n");
   }
   printf(inttostr(detectCPUSpeed()));
   printf("MHz CPU speed\n");
   srand(detectCPUSpeed());
   logf("Random seeded to CPU speed\n");
   printf(">>> ");
}

//first stage of booting, init core
void boot_stage1(){
    clear_screen();
    logf("Terminal initialized\n");
    printf("Ethan started to boot!\n");
    printf("GDT...\n");
    init_gdt();
    printf("OK\n");
    logf("GDT initialized\n");
    printf("PIC...\n");
    init_pic();
    printf("OK\n");
    logf("PIC initialized\n");
    printf("IDT...\n");
    init_idt();
    printf("OK\n");
    logf("IDT initialized\n");
    printf("Keyboard...\n");
    init_keyboard();
    printf("OK\n");
    logf("Keyboard intialized\n");
    logf("IDT and irq handlers operational\n");
   printf("Initializing paging...\n"); //only problems.... for now
   init_paging();
   printf("OK\n");
   logf("Paging initialized\n");
   printf("Timer...\n");
   init_timer(TIMER_FREQUENCY);
   printf("OK\n");
   logf("Timer initialized\n");
   printf("Time-of-day clock...\n");
   rtc_install();
   printf("OK\n");
   printf("Entering second stage...\n");
   boot_stage2();
}
