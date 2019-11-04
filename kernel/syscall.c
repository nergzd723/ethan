#include <isr.h>
#include <logger.h>

void syscall_handler(){
    logf("Syscall!\n");
}

void install_syscall_handler(){
    register_interrupt_handler(149, &syscall_handler);
}
