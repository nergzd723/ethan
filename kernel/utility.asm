;; Contains asm stubs to help cpu context setup

global gdt_flush
gdt_flush:
   mov eax, [esp+4]  ; eax = gdt_ptr
   lgdt [eax]        ; load the GDT pointer

   mov ax, 0x10      ; 0x10 is kernel segment data descriptor offset
   mov ds, ax        
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax
   jmp 0x08:.flush   ; 0x08 is kernel segment code descriptor offset
                     ; far jump to .flush
.flush:
   ret


global idt_flush
idt_flush:
   mov eax, [esp+4]  ; eax = idt_ptr
   lidt[eax]         ; load the IDT ptr
   ret


global load_page_directory
load_page_directory:
   mov eax, [esp+4]
   mov cr3, eax
   ret

global entering_v86
; you should declare this function as :
; extern void entering_v86(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip);
entering_v86:
   mov ebp, esp               ; save stack pointer
   push dword  [ebp+4]        ; ss
   push dword  [ebp+8]        ; esp
   pushfd                     ; eflags
   or dword [esp], (1 << 17)  ; set VM flags
   push dword [ebp+12]        ; cs
   push dword  [ebp+16]       ; eip
   mov ax, 0x0013
   int 0x15
   or dword [esp], (0 << 17)  ; set VM flags
   iret

global enable_paging
enable_paging:
   mov eax, cr0
   or eax, 0x80000000
   mov cr0, eax
   ret

global cpu_cycle
cpu_cycle:
   xor eax,edx
   xor edx,eax
   xor eax,edx
   xor edx,eax
   xor eax,edx
   xor edx,eax
   xor eax,edx
   xor edx,eax
   xor eax,edx
   xor edx,eax
   xor eax,edx
   xor edx,eax
   xor eax,edx
   xor edx,eax
   xor eax,edx
   xor edx,eax
   ret

global writeshittoeax
writeshittoeax:
   mov eax, 0x1
   ret
   
global scall
scall:
   int 25
   ret