global shutdownasm
;will try to enter v86 mode and shut down with 0x15 int
[bits 32]
shutdownasm:
	cli
	pushfd
	or dword [esp], (1 << 17)
	call shutdownint

;calls BIOS interrupt 15 which SHOULD shutdown the computer
shutdownint:
    mov ax,0x5301
    xor bx, bx
    int 0x15
    mov ax, 0x530e
    xor bx, bx
    mov cx, 0x0102
    int 0x15
    mov ax, 0x5307
    mov bx, 0x0001
    mov cx, 0x0003
    int 0x15
    hlt