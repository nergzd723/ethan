#define PANIC 1

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
#include <shell.h>

void panic(char* panicmessage){
    cli();
    reset_cursor();
    printk("The system has crashed, because of %s\n", panicmessage);
    printk("Returning to main shell...\n");
    wait(1);
    reset_shell();
}
