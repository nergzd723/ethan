#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096

unsigned short memorycount();
unsigned char lowmem();
unsigned char himem();

#define halloc(a) heap_allocate(a)

uint32_t page_directory[1024] __attribute__((aligned(PAGE_SIZE)));
uint32_t first_page_table[1024] __attribute__((aligned(PAGE_SIZE)));
uint32_t second_page_table[1024] __attribute__((aligned(PAGE_SIZE)));
uint32_t upper_memory();
int lower_memory();

void disable_paging();
void init_paging(void);
void* heap_allocate(size_t size);

#endif