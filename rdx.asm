global _start

N equ 4                            

section .text
_start:
    mov rax, 2
    mov rbx, 3
    mov rcx, N
    imul rbx, rcx
    lea rdx, [rbx + rax]
    ret
    call exit
    
exit:
    mov rax, 60
    xor rdi, rdi
    syscall
    ret