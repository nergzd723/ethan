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
#include <boot.h>
#include "multiboot.h"


uint32_t upper_memory(){
   if (mbd->flags & MULTIBOOT_INFO_MEMORY) {
      return mbd->mem_upper;
   }
   return 0;
}
int lower_memory(){
   if (mbd->flags & MULTIBOOT_INFO_MEMORY) {
      return mbd->mem_lower;
   }
   return 0;
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

   if (mbd->flags & (1 << 7)) {
      /* Safely read the drive regions */
      // TODO
   }
   boot_stage1();
   //clear_screen(); really shouldnt
   //shutdown(); that doesnt work, is page faulting
   //panic("TEST_PANIC", 0); works!
   while(1) {
   }
}
