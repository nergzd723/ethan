[BITS 16]       ; We need 16-bit intructions for Real mode

[ORG 0x7C00]    ; The BIOS loads the boot sector into memory location 0x7C00
mov bp,0x9000
mov sp,bp
mov bx, msg
mov [BOOT_DRIVE],dl
STAGE2 equ 0x800
STAGE2_SECTORS equ 2+1
TRACKS equ 2
call print_string
call load_stage2
call STAGE2
load_stage2:
    call print_string
    mov cl, 2
    mov bx, STAGE2
    mov dh, 1
    mov dl, [BOOT_DRIVE]
load_sector:
    call disk_load
    cmp cl, STAGE2_SECTORS
    je loaded
    cmp cl, 15
    add cl, 1
    add bx, 512
    jmp load_sector
loaded:
    ret
disk_load:
    pusha
    push dx
    mov ah,0x02
    mov al,dh
    mov dh,0x0
    int 0x13
    jc disk_error
    pop dx
    cmp dh,al
    jne disk_error
    popa
    ret
disk_error:
    mov ah,0x0e
    mov al,'X'
    int 0x10
    mov bx,errMsg
    call print_string
    jmp $
print_string:
    pusha
    mov cx,bx
    mov ah,0x0e
    printStringStart:
    mov al,[bx]
    cmp al,0
    je done
    int 0x10
    inc bx
    jmp printStringStart
    done:
    popa
    ret
reset_drive:
        mov ah, 0               ; RESET-command
        int 13h                 ; Call interrupt 13h
        or ah, ah               ; Check for error code
        jnz reset_drive         ; Try again if ah != 0

        mov ax, 0
        mov es, ax
        mov bx, 0x1000          ; Destination address = 0000:1000

        mov ah, 02h             ; READ SECTOR-command
        mov al, 02h             ; Number of sectors to read = 1
        mov ch, 0               ; Cylinder = 0
        mov cl, 02h             ; Sector = 2
        mov dh, 0               ; Head = 0
        int 13h                 ; Call interrupt 13h
        or ah, ah               ; Check for error code
        jnz reset_drive         ; Try again if ah != 0

        cli                     ; Disable interrupts, we want to be alone

        xor ax, ax
        mov ds, ax              ; Set DS-register to 0 - used by lgdt

        lgdt [gdt_desc]         ; Load the GDT descriptor

        mov eax, cr0            ; Copy the contents of CR0 into EAX
        or eax, 1               ; Set bit 0
        mov cr0, eax            ; Copy the contents of EAX into CR0

        jmp 08h:clear_pipe      ; Jump to code segment, offset clear_pipe

msg db "Ethanium real mode interface", 0
[BITS 32]                       ; We now need 32-bit instructions
clear_pipe:
        mov ax, 10h             ; Save data segment identifyer
        mov ds, ax              ; Move a valid data segment into the data segment register
        mov ss, ax              ; Move a valid data segment into the stack segment register
        mov esp, 090000h        ; Move the stack pointer to 090000h
        jmp $
        jmp 08h:01000h          ; Jump to section 08h (code), offset 01000h


gdt:                    ; Address for the GDT

gdt_null:               ; Null Segment
        dd 0
        dd 0

gdt_code:               ; Code segment, read/execute, nonconforming
        dw 0FFFFh
        dw 0
        db 0
        db 10011010b
        db 11001111b
        db 0

gdt_data:               ; Data segment, read/write, expand down
        dw 0FFFFh
        dw 0
        db 0
        db 10010010b
        db 11001111b
        db 0

gdt_end:                ; Used to calculate the size of the GDT



gdt_desc:                       ; The GDT descriptor
        dw gdt_end - gdt - 1    ; Limit (size)
        dd gdt                  ; Address of the GDT

times 510-($-$$) db 0           ; Fill up the file with zeros
dw 0AA55h                ; Boot sector identifyer
