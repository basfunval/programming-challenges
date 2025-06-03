section .data
        instr db "enter number: ",0
        infmt db "%d",0
        outfmt db "factorial is: %d",10,0

section .bss
        num resq 1
        x resq 1
        total resq 1

section .text
global main

extern scanf
extern printf

%macro scanf 2 ;stdio scanf wrapper
        push rbp
        xor rax, rax
        lea rdi, [%1]
        lea esi, [%2]
        call scanf
        pop rbp
%endmacro

%macro printf 1 ;stdio printf wrapper      
        push rbp
        mov rax, 0
        lea rdi, [%1]
        call printf
        pop rbp
%endmacro

main:
        printf instr
        scanf infmt, num

        mov rax, [num]
        sub rax, 1
        mov [x], rax
        mov rax, [num]
        mov rbx, [x]
loop:
        mul rbx
        mov [total], rax ;total *= x
        mov rax, [x]
        sub rax, 1       ;--x
        jz  exit         ;if (x==0) exit
        mov [x], rax
        mov rax, [total]
        mov rbx, [x]
        jmp loop
exit:
        mov rsi, [total]
        printf outfmt
        
        mov rax, 60
        mov rdi, 0
        syscall
