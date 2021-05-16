global _start

max_length equ 101                   ; максимальная длина вводимой строки
codeUpDown equ 32                    ; разница верхнего и нижнего регистра символа

section .bss
var_read: resb max_length            ; массив для ввода  
var_write: resb max_length           ; массив для вывода
var_readb: resb 1                    ; байт для удобства


section .text
_start:
    call read
    xor R12, R12
    L1:                              ; цикл с помощью jump'ов
        mov R13b, [var_read+R12]

        cmp R13b, 90
        jbe lat_symbDown             ; проверка на латинские символы в верхнем регистре
        L2:                          ;и их последующее изменение при положительном результате
        
        cmp R13b, 122
        jbe lat_symbUp
        L3:
        
        cmp R13b, 223
        jbe cir_symbDown
        L4:
        
        cmp R13b, 255
        jbe cir_symbUp
        L5:

        mov [var_write+R12], R13b
        inc R12
        cmp R12, max_length
        jbe L1
    
    call write
    call exit
       
lat_symbDown:
    cmp R13b, 65
    jb L2
    add R13b, codeUpDown
    jmp L5
    
lat_symbUp:
    cmp R13b, 97
    jb L3
    sub R13b, codeUpDown
    jmp L5

cir_symbDown:
    cmp R13b, 192
    jb L4
    add R13b, codeUpDown
    jmp L5
    
cir_symbUp:
    cmp R13b, 224
    jb L5
    sub R13b, codeUpDown
    jmp L5

read			
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
    mov rdx, max_length
    syscall
    ret

exit:
    mov rax, 60
    xor rdi, rdi
    syscall
    ret 
