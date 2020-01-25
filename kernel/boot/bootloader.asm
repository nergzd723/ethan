org 0x0
section .text
bits 16
global start
start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov si, ax
    mov sp, 0x7c00
reset:
    mov ah, 0x00
    mov dl, 0x00
    int 0x13
    jc reset
    mov ax, 0x100
    mov es, ax
    xor bx, bx
    mov ah, 0x02
    mov al, 1
    mov ch, 1
    mov cl, 2
    mov dh, 0
    mov dl, 0
    int 0x13
    jc reset
    jmp 0x0:0x100

times 510-($-$$) db 0
dw 0xaa55