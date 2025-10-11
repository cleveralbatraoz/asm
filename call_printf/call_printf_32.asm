        global main
        extern printf

        section .rodata
format:
        db "%i %s%c", 10, 0
str:
        db "string", 0

        section .text

call_printf:
        push dword 'A'
        push dword str
        push dword 5
        push dword format
        call printf
        add esp, 16
        ret

main:
        call call_printf
        mov eax, 0
        ret

        section .note.GNU-stack
