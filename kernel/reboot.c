#include <keyboard.h>
#include <shutdown.h>
#include <io.h>
void reboot()
{
    asm volatile ("cli");
    outb(KBRD_INTRFC, KBRD_RESET);
loop:
    asm volatile ("hlt"); /* if that didn't work, halt the CPU */
    goto loop; /* if a NMI is received, halt again */
}

void shutdown_qemu_vm(){
    outb(0x604, 0x200);
}