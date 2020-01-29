jmp start

GDT:
dq 00000000000000000h    ;null 8-bytes entrie
dw 0FFFFh      ;limit low word
dw 00000h      ;base low word
db 000h          ;base middle byte
db 010011010b   ;acess byte (code, readable, system, unconforming)
db 011001111b   ;4 granularity (4Kb) bits and limit high 4 bits
db 000h             ;base high byte
dw 0FFFFh      ;limit low word     
dw 00000h      ;base low word
db 000h          ;base middle byte
db 010010010b   ;acess byte (data, writeable, system, unconforming)
db 011001111b  ;4 granularity (4Kb) bits and limit high 4 bits
db 000h            ;base high byte
GDTP:              ;GDT pointer declaration
dw GDTP-GDT    ;GDT limit = GDT end - GDT init - 1
dd GDT             ;32-bit offset of GDT

start:
    mov ax, 0x2000 ; we are loaded at 0x2000
    mov ds, ax
    mov es, ax
    mov ax, 0x1000 ; stack just below at 0x1000
    mov ss, ax
    xor sp, sp
    xor ah, ah
    mov al, 0x03 ; reset graphics mode to clear the screen
    int 10h
    mov si, welcome_str
    call print
    mov si, a20_str
    call print
    call enable_a20
    mov si, a20_str_end
    call print
    cli
    mov si, pmode_s
    call print
    lgdt[GDTP]
    mov eax, cr0
    or eax, 1
    mov cr0, eax ; protected mode
    mov ax,10h
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    jmp 08h:protected_ethjr

flush_gdt:
    ret

.halt:
    cli
    hlt
    jmp .halt

welcome_str: db 0x0d, 0x0a, "Ethanium jr. loaded!", 0
a20fail_str: db 0x0d, 0x0a, "Enabling line A20 failed. Ethanium can not proceed further.", 0
a20_str: db 0x0d, 0x0a, "Enabling line A20...", 0
a20_str_end: db 0x0d, 0x0a, "Line A20 successfully enabled. Now loading GDT...", 0
pmode_s: db 0x0d, 0x0a, "Done! Entering Protected Mode...", 0
print:
    mov ah, 14 ; bios tty print call: ah : 114, bx : page number
    xor bx, bx
    jmp .getchar
.repeatint:
    int 10h
.getchar:
    lodsb
    test al, al
    jnz .repeatint

.endofprint:
    ret

CheckA20:
pushf
push ds
push es
push di
push si
cli
xor ax, ax ; ax = 0
mov es, ax
not ax ; ax = 0xFFFF
mov ds, ax
mov di, 0x0500
mov si, 0x0510
mov al, byte [es:di]
push ax
mov al, byte [ds:si]
push ax
mov byte [es:di], 0x00
mov byte [ds:si], 0xFF
cmp byte [es:di], 0xFF
pop ax
mov byte [ds:si], al
pop ax
mov byte [es:di], al
mov ax, 0
je check_a20_exit
mov ax, 1
check_a20_exit:
pop si
pop di
pop es
pop ds
popf
ret   

check_a20__exit:
    pop si                        
    pop di
    pop es
    pop ds
    popf
    ret
Wait_8042_data:
  in      al,0x64
  test    al,1
  jz      Wait_8042_data
  ret ; wait for data buffer
Wait_8042_command:
  in      al,0x64
  test    al,2
  jnz     Wait_8042_command
  ret ;wait for combuffer
a20bios:
    mov ax, 0x2401
    int 15h
    ; actually very rarely supported but should try
    ret
a20_8042keyboard:
    cli
    call Wait_8042_command
    mov al, 0xAD
    out 0x64, al
    call Wait_8042_command
    mov al, 0xD0
    out 0x64, al
    call Wait_8042_data
    in al, 0x60
    push ax
    call Wait_8042_command
    mov al, 0xD1
    out 0x64, al
    call Wait_8042_command
    pop ax
    or al, 2
    out 0x64, al
    call Wait_8042_command
    mov al, 0xAE
    out 0x64, al
    call Wait_8042_command
    ret

a20_fast:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret
enable_a20:
    call CheckA20
    cmp ax, 0
    jne a20_done
    call a20bios
    call CheckA20
    cmp ax, 0
    jne a20_done
    call a20_8042keyboard
    call CheckA20
    cmp ax, 0
    jne a20_done
    call a20_fast
    call CheckA20
    xchg bx, bx ; basically nop
    cmp ax, 0
    jne a20_done
a20_fail:
    mov si, a20fail_str
    call print
    hlt

a20_done:
    ret
bits 32
protected_ethjr:
    mov eax, 0xDEADBEEF
    mov ax, 08h  ;set segment identifier (first segment after the null one)
    mov ds, ax       ;move 0008h from ax into ds
    mov ss, ax       ;move 0008h from ax into ss
    mov ebp,080000h       ;set the stack somewhere away from the code
    mov esp, 08FFFFh
    mov ax,0F01h            ;set ax to CGA character code (white smile)
    mov [0B8000h],ax      ;put ax on first character of CGA video
    hlt