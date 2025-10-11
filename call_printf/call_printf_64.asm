        default rel

        global main
        extern printf

        section .rodata
format:
        db "%i %s%c", 10, 0
str:
        db "string", 0

        section .text

call_printf:
        sub rsp, 8
        lea rdi, [format]
        mov esi, 5
        lea rdx, [str]
        mov ecx, 'A'
        call printf
        xor eax, eax
        add rsp, 8
        ret

main:
        push rbp
        mov rbp, rsp
        call call_printf
        pop rbp
        ret

        section .note.GNU-stack
