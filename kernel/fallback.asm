;; Video driver code - switches the CPU back into real mode
;; Then executes an int 0x10 instruction

; dont be fooled by how simple this looks, it took lots of 
; hours of frustration to get this to work right

%define REBASE(x)                          (((x) - reloc) + INT32_BASE)
global switchvm
switchvm:
jmp vid_start
save_idt: dd 0
          dw 0
save_gdt: dd 0
          dw 0
save_esp: dd 0
vid_mode: dw 0
vid_start:
cli
mov word [vid_mode],ax
mov [save_esp],esp
sidt [save_idt]
lidt [REBASE(idt16_ptr)]
sgdt [save_gdt]
lgdt [REBASE(gdt16_ptr)]
jmp 0x18:pmode

pmode:
mov ax,0x20
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax
mov ss,ax
mov eax,cr0
dec eax
mov cr0,eax
jmp 0:realmode1

realmode1:
xor ax,ax
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax
mov ss,ax
mov sp,0xf000
sti

;; first zero out the 256 byte memory for the return function from getmodeinfo
cld
;; ax is already zero! I just saved myself a few bytes!!
mov cx,129
mov di,0x5000
rep stosw

mov ax,[vid_mode]
xor ax,0x13
jnz svga_mode

;; Ok, just a regular mode13
mov ax,0x13
int 0x10
;; we didnt actually get a Vidmode structure in 0x5000, so we 
;; fake it with the stuff the kernel actually uses
mov word [0x5001],0xDD     ; mode attribs, and my favorite cup size
mov word [0x5013],320      ; width
mov word [0x5015],200      ; height
mov byte [0x501a],8        ; bpp
mov byte [0x501c],1        ; memory model type = CGA
mov dword [0x5029],0xa0000 ; screen memory
jmp done

svga_mode:

mov ax,0x4f01 ; Get mode info function
mov cx,[vid_mode]
or cx,0x4000 ; always try to use linear buffer
mov di,0x5001
int 0x10
mov [0x5000],ah
or ah,ah
jnz done

mov ax,0x4f02 ; Now actually set the mode
mov bx,[vid_mode]
or bx,0x4000
int 0x10

done:
cli
mov eax,cr0
inc eax
mov cr0,eax
jmp 0x8:pm1

pm1:
mov eax,0x10
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax
mov ss,ax
mov dword esp,[save_esp]
lidt [save_idt]
ret

	idt16_ptr:                               ; IDT table pointer for 16bit access
		dw 0x03FF                              ; table limit (size)
		dd 0x00000000                          ; table base address
		
	gdt16_base:                              ; GDT descriptor table
		.null:                                 ; 0x00 - null segment descriptor
			dd 0x00000000                        ; must be left zero'd
			dd 0x00000000                        ; must be left zero'd
			
		.code32:                               ; 0x01 - 32bit code segment descriptor 0xFFFFFFFF
			dw 0xFFFF                            ; limit  0:15
			dw 0x0000                            ; base   0:15
			db 0x00                              ; base  16:23
			db 0x9A                              ; present, iopl/0, code, execute/read
			db 0xCF                              ; 4Kbyte granularity, 32bit selector; limit 16:19
			db 0x00                              ; base  24:31
			
		.data32:                               ; 0x02 - 32bit data segment descriptor 0xFFFFFFFF
			dw 0xFFFF                            ; limit  0:15
			dw 0x0000                            ; base   0:15
			db 0x00                              ; base  16:23
			db 0x92                              ; present, iopl/0, data, read/write
			db 0xCF                              ; 4Kbyte granularity, 32bit selector; limit 16:19
			db 0x00                              ; base  24:31
			
		.code16:                               ; 0x03 - 16bit code segment descriptor 0x000FFFFF
			dw 0xFFFF                            ; limit  0:15
			dw 0x0000                            ; base   0:15
			db 0x00                              ; base  16:23
			db 0x9A                              ; present, iopl/0, code, execute/read
			db 0x0F                              ; 1Byte granularity, 16bit selector; limit 16:19
			db 0x00                              ; base  24:31
			
		.data16:                               ; 0x04 - 16bit data segment descriptor 0x000FFFFF
			dw 0xFFFF                            ; limit  0:15
			dw 0x0000                            ; base   0:15
			db 0x00                              ; base  16:23
			db 0x92                              ; present, iopl/0, data, read/write
			db 0x0F                              ; 1Byte granularity, 16bit selector; limit 16:19
			db 0x00                              ; base  24:31
			
	gdt16_ptr:                               ; GDT table pointer for 16bit access
		dw gdt16_ptr - gdt16_base - 1          ; table limit (size)
		dd gdt16_base                          ; table base address
times 512-($-$$) db 0