global _start

max_length equ 64                    ; максимальная длина вводимой строки
code_zero equ 48

section .bss
var_read: resb max_length            ; массив для ввода  
var_write: resb max_length           ; массив для вывода
var_readb: resb 1                    ; байт для удобства


section .text
_start:
    call read
    xor R14, R14
    xor R12, R12
    L2:                              ; цикл с помощью jump'ов
        mov R13b, [var_read+R12]
        inc R12

        cmp R13b, 0                  ; проверка на конец введённой строки
        je L5

        sub R13b, code_zero          ; проверка на равенство цифре
        cmp R13b, 9
        jbe L2

        xor R15, R15                 ; запись в var_write для вывода, в случае 
        mov R15b, [var_read+R12-1]   ;буквенного символа
        mov [var_write+R14], R15b
        inc R14
        jmp L2
    L5:
    
    xor al, al                       ; запись нуля в конец выводимой строки 
    mov [var_write+R14], al
    mov [var_readb], R14b            ; длина выводимой строки
    call write
    call exit
    

read:						
    mov rax, 0					
    mov rdi, 0 					
    mov rsi, var_read			
    mov rdx, max_length					
    syscall				
    ret	
    ret

write: 
    mov rax, 1
    mov rdi, 1
    mov rsi, var_write
    mov rdx, [var_readb]
    syscall
    ret
    
exit:
    mov rax, 60
    xor rdi, rdi
    syscall
    ret