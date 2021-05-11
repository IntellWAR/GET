global _start

code_zero equ 48

section .bss
var_readb: resb 1                    ; байт для удобства

section .text
_start:
    call read
    mov rax, [var_readb]
    sub rax, code_zero  
    call exit

read:						
    mov rax, 0					
    mov rdi, 0 					
    mov rsi, var_readb			
    mov rdx, 1				
    syscall				
    ret	
    
exit:
    mov rax, 60
    xor rdi, rdi
    syscall
    ret