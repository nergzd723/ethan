[bits 16]
global Entry16
Entry16:
	cli			; Disable interrupts.
    mov eax, 0
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov ss, eax
	and eax, 0x7FFFFFFe	; Disable paging bit & disable 16-bit pmode.
 
	jmp 0:GoRMode		; Perform Far jump to set CS.
 
GoRMode:
	mov ax, 0		; Reset segment registers to 0.
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

