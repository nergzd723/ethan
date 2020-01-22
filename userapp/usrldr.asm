section .text
extern rattata
dw 0xDACEA
mov eax, 0xDEADBEEF
call rattata
; if we ever return, we must try to exit the sequence via exit syscall or if it doesnt work just halt
mov eax, 2
int 19h
