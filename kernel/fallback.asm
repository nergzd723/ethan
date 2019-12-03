; A helper function for calling bios from protected mode
; Execute bios interrupts under given context
; Then Save the result context to a given address
SECTION .text
USE16

real:
    cli
    mov bx, unreal_gdt.data
    mov es, bx
    mov ds, bx
    and al,0xFE            ; back to realmode
    mov  cr0, eax          ; by toggling bit again
    mov ax, 0x1000
    mov ax, ss
    mov sp, 0xf000
    mov ax, 0x5307
    mov bx, 0x0001
    mov cx, 0x0003
    int 0x15

unreal_gdtr:
    dw unreal_gdt.end + 1  ; size
    dd unreal_gdt          ; offset

unreal_gdt:
.null equ $ - unreal_gdt
    dq 0
.data equ $ - unreal_gdt
    istruc GDTEntry
        at GDTEntry.limitl,        dw 0xFFFF
        at GDTEntry.basel,         dw 0x0
        at GDTEntry.basem,         db 0x0
        at GDTEntry.attribute,        db attrib.present | attrib.user | attrib.writable
        at GDTEntry.flags__limith, db 0xFF | flags.granularity | flags.default_operand_size
        at GDTEntry.baseh,         db 0x0
    iend
.end equ $ - unreal_gdt