#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <tty.h>
#include <idt.h>
#include <pic.h>
#include <io.h>
#include <logger.h>
#include <gdt.h>
#include <timer.h>
#include <keyboard.h>
#include <memory.h>
#include <panic.h>
#include <isr.h>
#include <defines.h>
#include <cpu_speed.h>
#include <random.h>
#include <multiboot.h>

void kmain(multiboot_info_t* mbd, uint32_t magic) {
   init_logger();
   logf("Logger initialized\n");

   /* Check if the bootloader magic is correct */
   if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
      logf("Magic is incorrect...: %x\n", magic);
      while(1); // TODO: change to panic
   }

   logf("mbd->flags: %x\n", mbd->flags);
   if (mbd->flags & MULTIBOOT_INFO_MEMORY) {
      logf("mem_lower = %x\n", mbd->mem_lower);
      logf("mem_upper = %x\n", mbd->mem_upper);
   }

   /* Log a map of the regions of memory */
   if (mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
      logf("mmap_addr = %x, mmap_length = %x\n", mbd->mmap_addr, mbd->mmap_length);
      multiboot_memory_map_t* map;

      for (map = (multiboot_memory_map_t*) mbd->mmap_addr;
            (uint32_t) map < (mbd->mmap_addr + mbd->mmap_length);
            map = (multiboot_memory_map_t*)((uint32_t) map + map->size + sizeof(map->size))) 
      {
         logf("base_addr= %x, length= %x, type = %x\n",
               map->addr_upper,
               map->addr_lower,
               map->len_lower,
               map->type);
      }

   }

   if (mbd->mods_count <= 0) {
      panic("Module counter shows no ramdisk present");
   }
   // Locate the ramdisk
   uint32_t initrd_start = *((uint32_t*) mbd->mods_addr);
   uint32_t initrd_end = *(uint32_t*)(mbd->mods_addr+4);
   logf("initrd_start: %x\n", initrd_start);
   logf("initrd_end: %x\n", initrd_end);

   // Make sure the ramdisk does not bleed past the kernel end
   if (initrd_end > KERNEL_END) {
      panic("Ramdisk is bleeding past the kernel area!");
   }

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
   printf("Initializing paging...\n");
   init_paging();
   printf("OK\n");
   logf("Paging initialized\n");
   printf("Timer...\n");
   init_timer(TIMER_FREQUENCY);
   printf("OK\n");
   logf("Timer initialized\n");
   printf("Entering second stage...\n");
   printf("Enabling hardware interrupts...\n");
   __asm__ volatile("sti");
   printf("OK\n");
   printf("Tasking...\n");
   init_tasking();
   printf("OK\n");
   logf("Tasking initialized\n");
   printf("Testing timer...");
   waitm(30);
   clear_screen();
   printf("Ethanium booted! Got ");
   printf(inttostr(memorycount()));
   printf("K mem total, ");
   printf(inttostr(lowmem()));
   printf("K lowmem, ");
   printf(inttostr(memorycount()-lowmem()));
   printf("K USABLE extended memory\n");
   if (memorycount() == 65535){
       printf("It seems that you have more than 64M of RAM. Ethanium may not work properly\n");
   }
   printf(inttostr(discoverspeed()));
   printf("MHz CPU speed\n");
   srand(discoverspeed());
   logf("Random seeded to CPU speed\n");
   printf(">>> ");
   //clear_screen(); really shouldnt
   //shutdown(); that doesnt work, is page faulting
   //panic("TEST_PANIC", 0); works!
   while(1) {
   }
}
