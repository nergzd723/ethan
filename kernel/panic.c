#define PANIC

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <idt.h>
#include <pic.h>
#include <io.h>
#include <logger.h>
#include <io.h>
#include <frame_buffer.h>
#include <timer.h>

void panic(char* panicmessage, int paniccode){
    waitm(1000);
    clear_screen();
    fill_screen(FB_MAGENTA);
    printf("System crash!\n");
    printf(inttostr(paniccode));
    printf("\n");
    logf("clearing interrupts");
    cli();
    halt();
}