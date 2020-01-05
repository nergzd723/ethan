KERNEL equ 0x1000
KERNEL_SECTORS equ 24

mov bx, msgStage2
call print_string

call load_kernel

mov bx, msg
call print_string

int 0x12
mov [0x600], ax

call switch_to_pm

%include 'bios.asm'

%include 'gdt.asm'
%include 'protected_mode.asm'
%include 'print32.asm'

[bits 16]

load_kernel:
    mov bx, msgKernel
    call print_string
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
    jc error1
    cmp al, 1
    jne error2
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

error1:
    mov bx, errorMsg1
    call print_string
    jmp $

error2:
    mov bx, errorMsg2
    call print_string
    jmp $

[bits 32]

BEGIN_PM:
    mov ebx, msgProt
    call print_string32
    call KERNEL
    jmp $

BytesPerSector equ 512
NumHeads equ 2
SectorsPerTrack equ 18

Sector db 0

BOOT_DRIVE db 0
msgStage2 db "Stage 2 reached!", 0
msgProt db "Successfully switched to 32-bit mode",0
msgKernel db "Loading the kernel onto memory",0
msg db "Loaded!!", 0
errorMsg1 db "Error1", 0
errorMsg2 db "Error2", 0

times 1024-($-$$) db 0