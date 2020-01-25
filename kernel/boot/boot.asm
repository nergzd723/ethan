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
    mov esp, eax ; save eax
    lgdt[GDT_V] ; set up hardcoded GDT
    mov ax, 8
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov eax, esp ; restore it
    mov esp, kernel_stack_top       ; Set up the stack
    push eax;                       ; Push multiboot header
    push ebx;                       ; Push multiboot magic
    call 0x10:kmain                 ; Far call to kmain (never to return) sets up CS as well

align 16
GDT_T: 
    dd 0, 0 ;null descriptor
    dd 0000FFFFh, 008F9200h
    dd 0000FFFFh, 00CF9A00h
GDT_V:
    dw GDT_V-GDT_T
    dd GDT_T
