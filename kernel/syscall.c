#include <isr.h>
#include <logger.h>
#include <frame_buffer.h>
#include <shell.h>
#include <logger.h>

void syscall_handler(context_t* cpustate)
{
    logf("syscall: got a call: %x\n", cpustate->eax);
    switch (cpustate->eax)
    {
    case 7828067:
        char*& a = cpustate->edx;
        printf(a);
        break;
    case 6451819:
        asm volatile ("sti");
        printf("Process closed");
        logf("syscall: trying to breakthrough to shell!");
        reset_shell();
    case 0x1:
        printf("\nIndeed\n");
        break;
    default:
        logf("syscall: bad syscall\n");
        break;
    }
}

void install_syscall_handler(){
    register_interrupt_handler(25, &syscall_handler);
}