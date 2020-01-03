[bits 16]
jmp _start
GDTR:
   dw   0xFFFF      ;GDT Segment Limit
   dd   0x00400000   ;GDT Segment Base Address

IDTR:
   dw   0x07FF      ;IDT Segment Limit
   dd   0x00410000   ;IDT Segment Base Address

GDT:
   db   0x00      ;--0x0000 Null Descriptor-- Bits 07-00 of Limit
   db   0x00      ;Bits 15-08 of Limit
   db   0x00      ;Bits 07-00 of Base
   db   0x00      ;Bits 15-08 of Base
   db   0x00      ;Bits 23-16 of Base
   db   0xF0      ;Access Byte
   db   0x00      ;Flags and Bits 19-16 of Limit
   db   0x00      ;Bits 32-24 of Base

   db   0xFF      ;--0x0008 Kernel Code Segment Descriptor--
   db   0x00
   db   0x00
   db   0x00
   db   0x10
   db   0x98
   db   0xC0
   db   0x00

   db   0xFF      ;--0x0010 Kernel Data Segment Descriptor--
   db   0x00
   db   0x00
   db   0x00
   db   0x10
   db   0x92
   db   0xC0
   db   0x00

   db   0x8F      ;--0x0018 Stack Segment Descriptor--
   db   0xFF
   db   0xFF
   db   0xFF
   db   0x09
   db   0x96
   db   0xCF
   db   0x00

   db   0xFF      ;--0x0020 Video RAM Segment Descriptor--
   db   0xFF
   db   0x00
   db   0x00
   db   0x0A
   db   0x92
   db   0x41
   db   0x00

   db   0xFF      ;--0x0028 Bootstrap Code Segment Descriptor--
   db   0xFF
   db   0x00
   db   0x00
   db   0x02
   db   0x98
   db   0x00
   db   0x00

   db   0xFF      ;--0x0030 Bootstrap Data Segment Descriptor--
   db   0xFF
   db   0x00
   db   0x00
   db   0x02
   db   0x92
   db   0x00
   db   0x00
EGDT:
_start:
    ; loading point in REAL mode
    mov ax, 07C0h
    ; data segment to loading point
    mov ds, ax
    call print
    string db "Ethanium real mode interface", 0
print:
    mov ah, 0Eh ; int 0x10h requires it
.loop:
    ; print string using bios!
    lodsb
    cmp al, 0
    je .done
    int 10h
    jmp .loop
.done:
    ; enable line a20 - fucking paging!
    mov ax, 2401h
    int 15h
    ; to pmode!
    cli
    ; load gdt for real mode
    
    mov eax, cr0
    or al, 1
    mov cr0, eax
    ; yaaay! in 16bit pmode now, jumping to 32bit is not possible without gdt

times 510-($-$$) db 0
dw 0xAA55