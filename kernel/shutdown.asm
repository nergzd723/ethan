global shutdownasm

[BITS 32]
shutdownasm:
   cli

   mov   eax,cr0
   and   eax,0x7FFFFFFF
   mov   cr0,eax
   xor   eax,eax
   mov   cr3,eax

   jmp   0x0008:prot16

[BITS 16]

prot16:

   mov   ax,0x0010
   mov   ds,ax
   mov   es,ax
   mov   fs,ax
   mov   gs,ax
   mov   ss,ax

   mov   ax,0x03FF
   mov   eax,cr0
   and   eax,0xFFFFFFFE
   mov   cr0,eax

   jmp   dword 0x0000:real16

real16:

   xor   ax,ax
   mov   ds,ax
   mov   es,ax
   mov   fs,ax
   mov   gs,ax
   mov   ss,ax

   mov   ax,5304h
   sub   bx,bx
   int   15h

   mov   ax,5301h
   sub   bx,bx
   int   15h

   mov   ax,5308h
   mov   bx,1
   mov   cx,bx
   int   15h

   mov   ax,530Dh
   mov   bx,1
   mov   cx,bx
   int   15h

   mov   ax,530Fh
   mov   bx,1
   mov   cx,bx
   int   15h

   mov   ax,530Eh
   sub   bx,bx
   mov   cx,102h
   int   15h

   mov   ax,5307h
   mov   bx,1
   mov   cx,3
   int   15h