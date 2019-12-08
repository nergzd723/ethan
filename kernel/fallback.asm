use32
global .fall
.fall:
   cli
   mov eax, cr0
   and eax, 0x7FFFFFFF      ; disable paging
   mov cr0, eax

   jmp 0x18:.pmode16

use16

.pmode16:
   mov ax, 0x20
   mov ss, ax
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   mov eax, cr0
   and eax, 0xFFFFFFFE      ; disable protection
   mov cr0, eax1
   jmp .real_mode

.real_mode:
   mov ax, REAL_MODE_SEGMENT
   mov ss, ax
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov sp, REAL_MODE_STACK
   lidt [.bios_ivt]
   mov ax, 0x4F02
   mov bx, 0x4118
   int 10
   jmp .afterwards

extern boot_stage1
.afterwards:
    mov eax, cr0
    inc eax
    mov cr0, eax
    jmp boot_stage1

.bios_ivt:      ; this really is an IDTR for the BIOS
   .limit      dw 0x3FF
   .base      dd 0