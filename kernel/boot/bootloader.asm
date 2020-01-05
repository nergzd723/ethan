[org 0x7c00]

STAGE2 equ 0x800

STAGE2_SECTORS equ 2+1
TRACKS equ 2

%include 'kernel/boot/bios.asm'

%include 'kernel/boot/gdt.asm'
%include 'kernel/boot/protected_mode.asm'

mov [BOOT_DRIVE],dl

mov bp,0x9000
mov sp,bp


call load_stage2

call STAGE2

jmp $


[bits 16]
load_stage2:
    mov cl, 2
    mov bx, STAGE2
    mov dh, 1
    mov dl, [BOOT_DRIVE]
loads_sector:
    call disk_load
    cmp cl, STAGE2_SECTORS
    je loadedr
    cmp cl, 15
    add cl, 1
    add bx, 512
    jmp loads_sector
loadedr:
    ret
KERNEL equ 0x1000
KERNEL_SECTORS equ 24


call load_kernel

int 0x12
mov [0x600], ax

call switch_to_pm
[bits 16]

load_kernel:
    mov ax, 3
    mov cl, 4
    mov ch, 0
    mov bx, KERNEL
    mov dl, [BOOT_DRIVE]
    mov dh, 0
    mov ch, 0
load_sector:
    mov ah, 0x02
    mov al, 1
    int 0x13
    push bx
    mov bl, [Sector]
    cmp bl, KERNEL_SECTORS
    pop bx
    je loaded
    push bx
    mov bl, [Sector]
    inc bl
    mov [Sector], bl
    pop bx
    inc cl
    cmp cl, 18
    jne continue
    add ch, 1
    mov cl, 1
continue:
    add bx, BytesPerSector
    jmp load_sector
loaded:
    ret

[bits 32]

BEGIN_PM:
    call KERNEL
    jmp $

BytesPerSector equ 512
NumHeads equ 2
SectorsPerTrack equ 18

Sector db 0

BOOT_DRIVE db 0

times 510-($-$$) db 0
dw 0xaa55