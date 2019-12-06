#include <isr.h>
#include <logger.h>
#include <frame_buffer.h>
#include <logger.h>

void syscall_handler(context_t* cpustate)
{
    logf("syscall: got a call: %x\n", cpustate->eax);
    switch (cpustate->eax)
    {
    case 1:
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