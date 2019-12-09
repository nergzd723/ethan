;; Video driver code - switches the CPU back into real mode
;; Then executes an int 0x10 instruction

; dont be fooled by how simple this looks, it took lots of 
; hours of frustration to get this to work right

global switchvm
switchvm:
jmp vid_start
save_idt: dd 0
          dw 0
save_esp: dd 0
vid_mode: dw 0
vid_start:
cli
mov word [vid_mode],ax
mov [save_esp],esp
sidt [save_idt]
lidt[idt16_ptr]
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
idt16_ptr:                                 ; IDT table pointer for 16bit access
        dw 0x03FF                              ; table limit (size)
        dd 0x0000                          ; table base address
        dw 0x0000

times 512-($-$$) db 0