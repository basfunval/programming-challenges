section .data
        vexp dd 0.16
        a dd 35.74
        b dd 0.6215
        c dd 0.4275
        string: db "%d degrees fahrenheit",10,0
        tempstr: db "enter temperature: ",0
        windstr: db "enter windspeed: ",0
        floatfmt: db "%f",0

section .bss
        res: resq 1
        txt: resd 1
        v: resd 1
        t: resd 1

section .text
global main

extern printf
extern scanf

%macro exp 2 ;floating point exponential
        fld dword %2
        fld dword %1
        fyl2x
        fld1
        fld st1
        fprem
        f2xm1
        fadd
        fscale
        fxch st1
        fstp st0
        fstp qword [res]
%endmacro

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
        finit

        printf tempstr
        scanf floatfmt, t
        printf windstr
        scanf floatfmt, v

        ;temp = 35.74 + 0.6215*t + 35.75*v^0.16 + 0.4275*t*v^0.16
        exp [v], [vexp]
        cvtsd2ss xmm0, qword [res]
        movss [res], xmm0
        movss xmm0, [b]
        movss xmm1, [t]
        mulss xmm0, xmm1
        movss xmm1, [a]
        addss xmm0, xmm1
        movss xmm1, [a]
        movss xmm2, [res]
        mulss xmm1, xmm2
        movss xmm2, [c]
        movss xmm3, [t]
        mulss xmm2, xmm3
        movss xmm3, [res]
        mulss xmm2, xmm3
        addss xmm0, xmm2
        subss xmm0, xmm1

        ;store result in variable txt
        cvtss2si eax, xmm0
        mov [txt], eax

        ;printf temperature
        push rbp
        mov rax, 0
        lea rdi, [string]
        mov esi, [txt]
        call printf
        pop rbp
        
        ;exit 0 syscall
        mov rax, 60
        mov rdi, 0
        syscall
