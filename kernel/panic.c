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
    cli();
    reset_cursor();
    clear_screen();
    printf("                                System crash!\n\n\n\n\n");
    printf("                                ");
    printf(panicmessage);
    printf("\n\n\n\n");
    printf(inttostr(paniccode));
    printf("\n");
    halt();
}
