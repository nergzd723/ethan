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
unsigned char g_90x30_text[] =
{
/* MISC */
	0xE7,
/* SEQ */
	0x03, 0x01, 0x03, 0x00, 0x02,
/* CRTC */
	0x6B, 0x59, 0x5A, 0x82, 0x60, 0x8D, 0x0B, 0x3E,
	0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x2D, 0x10, 0xE8, 0x05, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x0C, 0x00, 0x0F, 0x08, 0x00,
};
unsigned char g_90x60_text[] =
{
/* MISC */
	0xE7,
/* SEQ */
	0x03, 0x01, 0x03, 0x00, 0x02,
/* CRTC */
	0x6B, 0x59, 0x5A, 0x82, 0x60, 0x8D, 0x0B, 0x3E,
	0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x2D, 0x08, 0xE8, 0x05, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x0C, 0x00, 0x0F, 0x08, 0x00,
};
void boot_stage2(){
   printf("Enabling hardware interrupts...\n");
    asm volatile("sti");
   printf("OK\n");
   printf("SoundBlaster16...\n");
   sb16_init();
   printf("ACPI...\n");
   initAcpi();
   printf("OK\n");
   logf("Paging initialized\n");
   clear_screen();
   logf("90x30 text mode applying\n");
   write_regs(g_90x30_text);
   printf("Ethanium booted! Got ");
   printf(inttostr(upper_memory()+lower_memory()));
   printf("K mem total, ");
   printf(inttostr(lower_memory()));
   printf("K lowmem, ");
   printf(inttostr(upper_memory()));
   printf("K USABLE extended memory\n");
   printf("Running on ");
   printf(detectBIOSVendor());
   printf(" machine\n");
   if (memorycount() > 65535){
       printf("It seems that you have more than 64M of RAM. Ethanium may not work properly\n");
   }
   printf(inttostr(detectCPUSpeed()));
   printf("MHz CPU speed\n");
   srand(discoverspeed());
   logf("Random seeded to CPU speed\n");
   printf(">>> ");
   __init_done = true;
   init_paging();
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
   printf("Timer...\n");
   init_timer(TIMER_FREQUENCY);
   printf("OK\n");
   logf("Timer initialized\n");
   printf("Time-of-day clock...\n");
   rtc_install();
   printf("OK\n");
   printf("Detecting SMBIOS...\n");
   unsigned char *mem = (unsigned char *) 0xF0000;
   int length, i;
   unsigned char checksum;
	while ((unsigned int) mem < 0x100000)
	{
		if (memcmp(mem,"_SM_",4)==0)
		{
			length = mem[5];
			checksum = 0;
			for(i = 0; i < length; i++)
			{
				checksum += mem[i];
			}
			if(checksum == 0) break;
		}
		mem += 16;
	}
	if ((unsigned int) mem == 0x100000)
	{
		logf("error: SMBIOS not found!");
		
	}
	smb_ep = (struct SMBIOSEntryPoint*)mem;
	logf("SMBIOS found at 0x%08x.");
    printf("OK\n");
   printf("Entering second stage...\n");
   boot_stage2();
}
