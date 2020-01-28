jmp start

gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

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

.halt:
    cli
    hlt
    jmp .halt

welcome_str: db 0x0d, 0x0a, "Ethanium jr. loaded!", 0
a20fail_str: db 0x0d, 0x0a, "Enabling line A20 failed. Ethanium can not proceed further.", 0
a20_str: db 0x0d, 0x0a, "Enabling line A20...", 0
print:
    mov ah, 14 ; bios tty print call: ah : 114, bx : page number
    xor bx, bx
    jmp .getchar
.repeatint:
    int 10h
.getchar:
    lodsb
    test al, al
    jnz .repeat
.endofprint:
    ret

CheckA20:
    pushf
    push ds
    push es                
    push di
    push si

    cli

    xor ax, ax                  
    mov es, ax
    mov di, 0x0500

    mov ax, 0xffff               
    mov ds, ax
    mov si, 0x0510

    mov al, es:[di]       
    push ax                      

    mov al, ds:[si]       
    push ax                       

    mov es:[di], 0x00   
    mov ds:[si], 0xFF 

    cmp es:[di], 0xFF     
    pop ax
    mov ds:[si], al  

    pop ax
    mov es:[di], al      

    mov ax, 0
    je check_a20__exit            

    mov ax, 1          

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
    jmp .halt

a20_done:
    ret