global printf
printf:
  mov eax, 1
  mov ebx, [esp+4]
  int 19h
  ret
global printb
printb:
  mov eax, 3
  mov ebx, [esp+4]
  int 19h
  ret
global exit
exit:
   mov eax, 2
   int 19h
   ret
global testsyscalls
testsyscalls:
   mov eax, 777
   ret
global scall
scall:
   int 19h
   ret
   
global write_eax
write_eax:
   push ebp
   mov ebp, esp
   mov eax, [ebp+8]
   pop ebp
   ret
   
global write_edx
write_edx:
   push ebp
   mov ebp, esp
   mov edx, [ebp+8]
   pop ebp
   ret