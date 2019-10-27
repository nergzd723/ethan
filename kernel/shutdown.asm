global shutdown
;will try to enter v86 mode and shut down with 0x15 int
[bits 32]
shutdown:
	cli
	pushfd
	or dword [esp], (1 << 17)
	call shutdownint

;calls BIOS interrupt 15 which SHOULD shutdown the computer
shutdownint:
    mov ax, 0x1000
    mov ax, ss
    mov sp, 0xf000
    mov ax, 0x5307
    mov bx, 0x0001
    mov cx, 0x0003
    int 0x15
    hlt