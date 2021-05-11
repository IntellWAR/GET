global _start

N equ 4                              ; число разрядов вводимого числа
code_zero equ 48

section .bss
var_read: resb N                     ; N байт для удобства

section .text
_start:
    call read
    call StringToDec
    call exit
    
StringToDec:
    xor rcx, rcx
    xor rax, rax
    xor rdx, rdx
    xor R8, R8
    L1:
        mov R8d, 10
        mul R8d
        mov dl, [var_read+rcx]
        inc rcx
        sub dl, code_zero
        add al, dl
        cmp rcx, N
        jb L1
     ret

read:				
    mov rax, 0					
    mov rdi, 0 					
    mov rsi, var_read		
    mov rdx, N
    syscall				
    ret	
    
exit:
    mov rax, 60
    xor rdi, rdi
    syscall
    ret