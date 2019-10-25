#define PANIC

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
#include "multiboot.h"
#include <frame_buffer.h>

void panic(char* panicmessage, int paniccode){
    clear_screen();
    fill_screen(FB_MAGENTA);
    printf("mbd->flags: %x\n", mbd->flags);
    if (mbd->flags & MULTIBOOT_INFO_MEMORY) {
      printf("mem_lower = %x\n", mbd->mem_lower);
      printf("mem_upper = %x\n", mbd->mem_upper);
   }

   /* Log a map of the regions of memory */
   if (mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
      printf("mmap_addr = %x, mmap_length = %x\n", mbd->mmap_addr, mbd->mmap_length);
      multiboot_memory_map_t* map;

      for (map = (multiboot_memory_map_t*) mbd->mmap_addr;
            (uint32_t) map < (mbd->mmap_addr + mbd->mmap_length);
            map = (multiboot_memory_map_t*)((uint32_t) map + map->size + sizeof(map->size))) 
      {
         printf("base_addr_high = %x, base_addr_low = %x, length_high = %x, length_low = %x, type = %x\n",
               map->addr_upper,
               map->addr_lower,
               map->len_upper,
               map->len_lower,
               map->type);
      }
      printf("System crash! Memory segments displayed\n");
      logf("clearing interrupts");
      cli();
      halt();
   }
}