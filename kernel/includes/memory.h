#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>

#define PAGE_SIZE 4096

unsigned short memorycount();
unsigned char lowmem();
unsigned char himem();

void init_paging(void);

#endif