#include <isr.h>
#include <logger.h>
#include <frame_buffer.h>
#include <logger.h>

void syscall_handler(context_t* cpustate)
{
    logf("syscall: got a call: %x", cpustate->eax);
    switch (cpustate->eax)
    {
    case 1:
        printf("\nIndeed\n");
        break;
    
    default:
        logf("syscall: bad syscall");
        break;
    }
}

void install_syscall_handler(){
    register_interrupt_handler(19, &syscall_handler);
}