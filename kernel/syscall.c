#include <isr.h>
#include <logger.h>
#include <frame_buffer.h>
#include <logger.h>

void syscall_handler(context_t* cpustate)
{
    logf("syscall: got a call: %x\n", cpustate->eax);
    switch (cpustate->eax)
    {
    case 6451819:
        logf("syscall: trying to breakthrough to shell!");
        clear_screen();
        printf("Process closed");
        for(;;);
    case 0x001:
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