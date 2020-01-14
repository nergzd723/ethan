#include <isr.h>
#include <logger.h>
#include <frame_buffer.h>
#include <shell.h>
#include <io.h>
#include <logger.h>

void syscall_handler(context_t* cpustate)
{
    logf("syscall: got a call: %x\n", cpustate->eax);
    switch (cpustate->eax)
    {
    // eax=1, ebx=char* printf
    case 1:
        printk((char*) cpustate->ebx);
        break;
    // eax=2, exit to the main kernelspace shell
    case 2:
        asm volatile ("sti");
        logf("Closing process");
        reset_shell();

    // eax=3, write single byte to screen
    case 3:
        fb_write_byte((char) cpustate->ebx);
        break;
    // eax=4, returns edx=1
    case 4:
        write_edx(1);
        break;
    // eax=777, test syscall
    case 777:
        printk("\nIndeed\n");
        break;
    default:
        logf("syscall: bad syscall\n");
        break;
    }
}

void install_syscall_handler(){
    register_interrupt_handler(25, &syscall_handler);
}