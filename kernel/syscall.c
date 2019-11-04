#include <isr.h>

void install_syscall_handler(){
    register_interrupt_handler(149, &syscall_handler);
}
void syscall_handler(){
    logf("Syscall!\n")
}