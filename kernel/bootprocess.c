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
#include <vga.h>
#include <rtc.h>
#include <sb16.h>
#include <v8086m.h>
#include <smbios.h>
#include <string.h>
#include <acpi.h>
#include <sysinit.h>
#include <shell.h>
#include <vmodes.h>
void garbage_handler(){
    reset_shell();
}
void boot_stage2(){
   printk("Enabling hardware interrupts...\n");
    asm volatile("sti");
   printk("OK\n");
   printk("SoundBlaster16...\n");
   sb16_init();
   printk("ACPI...\n");
   initAcpi();
   printk("OK\n");
   clear_screen();
   logf("80x25 text mode applying\n");
   write_regs(g_80x25_text, false);
   //vmode();
   //clear_screen_a();
   register_interrupt_handler(0x6, &garbage_handler);
   printk("Ethanium booted! Got ");
   printk(inttostr(upper_memory()+lower_memory()));
   printk("K mem total, ");
   printk(inttostr(lower_memory()));
   printk("K lowmem, ");
   printk(inttostr(upper_memory()));
   printk("K USABLE extended memory\n");
   printk("Running on ");
   printk(detectBIOSVendor());
   printk(" machine\n");
   if (memorycount() > 65535){
       printk("It seems that you have more than 64M of RAM. Ethanium may not work properly\n");
   }
   printk(inttostr(detectCPUSpeed()));
   printk("MHz CPU speed\n");
   srand(discoverspeed());
   logf("Random seeded to CPU speed\n");
   printk(">>> ");
   __init_done = true;
}

//first stage of booting, init core
void boot_stage1(){
    clear_screen();
    logf("Terminal initialized\n");
    printk("Ethan started to boot!\n");
    printk("GDT...\n");
    init_gdt();
    printk("OK\n");
    logf("GDT initialized\n");
    printk("PIC...\n");
    init_pic();
    printk("OK\n");
    logf("PIC initialized\n");
    printk("IDT...\n");
    init_idt();
    printk("OK\n");
    logf("IDT initialized\n");
    printk("Keyboard...\n");
    init_keyboard();
    printk("OK\n");
    logf("Keyboard intialized\n");
    logf("IDT and irq handlers operational\n");
   printk("Timer...\n");
   init_timer(TIMER_FREQUENCY);
   printk("OK\n");
   logf("Timer initialized\n");
   printk("Time-of-day clock...\n");
   rtc_install();
   printk("OK\n");
   printk("Detecting SMBIOS...\n");
   detect_SM_();
   logf("SMBIOS found at 0x%08x.");
    printk("OK\n");
   printk("Entering second stage...\n");
   boot_stage2();
}
