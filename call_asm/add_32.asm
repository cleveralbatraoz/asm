        global add

        section .text

add:
        push ebp
        mov ebp, esp
        mov eax, [ebp + 8]
        add eax, [ebp + 12]
        pop ebp
        ret
