global _start

N equ 7                            

section .text
_start:
    mov rdx, 1
    mov rax, 0
    lea rcx, [N - 1] 
    L1:
        add rax, rdx
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