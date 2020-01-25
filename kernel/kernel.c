#include <stdint.h>
#include <stddef.h>
#include <string.h>
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
#include <frame_buffer.h>
#include <boot.h>
#include <syscall.h>
#include "multiboot.h"

uint32_t upper_mem = 0;
int lower_mem = 0;


uint32_t upper_memory(){
   return upper_mem;
}
int lower_memory(){
   return lower_mem;
}
uint32_t apps[10];
void run(unsigned app)
{
   typedef void (*appp_t)(void);
   appp_t start_app = (appp_t)0x000100A8;
   start_app();
}
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
      lower_mem = mbd->mem_lower;
      logf("mem_upper = %x\n", mbd->mem_upper);
      upper_mem = mbd->mem_upper;
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
   init_gdt();
   init_idt();
   install_syscall_handler();
   init_keyboard();
   asm volatile("sti");
   typedef void (*appp_t)(void);
   appp_t start_app = (appp_t)mbd->mods_addr;
   start_app();
   boot_stage1();
   while(1){}
}
}