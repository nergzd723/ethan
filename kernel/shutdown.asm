global shutdown

shutdown:
	cli
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov ss, eax
 
	mov eax, cr0
	and eax, 0x7FFFFFFe
	mov cr0, eax
 
	jmp 0:realmode

realmode:
	mov sp, 0x8000
	mov ax, 0
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
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