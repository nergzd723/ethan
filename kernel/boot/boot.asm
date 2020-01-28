bits 32


;------------------
; Multiboot Header
;------------------
; For details: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-layout

MAGIC equ 0x1BADB002                ; Multiboot Magic
FLAGS equ 0                         ; We don't need any flags right now
CHECKSUM equ -(MAGIC + FLAGS)       ; checksum + magic + flags = 0

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

;---------------
; Kernel Stack
;---------------
section .bss
align 4
kernel_stack_bottom: equ $          ; Create a 16KB stack for the kernel
	resb  131072; 128 KB
kernel_stack_top:

;--------------------
; Kernel Entry Point
;--------------------
section .text
global start
extern kmain
start:
    cld                             ; Clear the direction flag for string operations
    mov esp, kernel_stack_top       ; Set up the stack
    push eax;                       ; Push multiboot header
    push ebx;                       ; Push multiboot magic
    call kmain                 ; Far call to kmain (never to return) sets up CS as well

align 16
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

