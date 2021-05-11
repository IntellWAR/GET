global _start

N equ 8                            

section .text
_start:
    mov rdx, 1
    mov rax, 1
    lea rcx, [N - 1] 
    L1:
        lea rax, [rdx + rax*2]
        call swap
        loop L1
    call exit
    
swap:
    xor rbx, rbx
    mov rbx, rdx
    mov rdx, rax
    mov rax, rbx 
    ret
    
exit:
    mov rax, 60
    xor rdi, rdi
    syscall
    ret