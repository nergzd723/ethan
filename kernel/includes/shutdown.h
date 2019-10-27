//nasm shutdown: disable interrupts, paging, a20, far jump to real mode, use bios 0x15 interrupt to shutdown
extern void shutdown();
void reboot();
