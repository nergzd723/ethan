ENTRY(start)
KERNEL_START_PMA = 0x00100000;
SECTIONS {
	. = KERNEL_START_PMA;
	.text BLOCK(16K) : ALIGN(4K)
	{
        *(.multiboot)
		*(.text)
	}
    
    .rodata :
    {
    	*(.rodata)
    }

    .data :
    {
    	*(.data)
    }

    .bss :
    {
    	*(.bss)
    }
}

