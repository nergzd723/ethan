#define PANIC

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <tty.h>
#include <idt.h>
#include <pic.h>
#include <io.h>
#include <logger.h>
#include <io.h>
#include <frame_buffer.h>

void panic(char* panicmessage, int paniccode){
    clear_screen();
    fill_screen(FB_MAGENTA);
    printf("System crash!\n");
    logf("clearing interrupts");
    cli();
    halt();
   }
}