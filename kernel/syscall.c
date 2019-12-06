#include <isr.h>
#include <logger.h>

void syscall_handler(){
    logf("something");
    for(;;);
}

void install_syscall_handler(){
    register_interrupt_handler(25, &syscall_handler);
}
