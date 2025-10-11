        global _Z3addii
        global _ZNK5Adder3addEi
        global _Z11call_squarePK1Ti
        global add

        extern _ZNK1T6squareEi

        section .text

_Z3addii:
        add rdi, rsi
        mov rax, rdi
        ret

_ZNK5Adder3addEi:
        mov rax, [rdi]
        add rax, rsi
        ret

; extern int32_t call_square(T const *t, int32_t arg);
_Z11call_squarePK1Ti:
        push rbp
        mov rbp, rsp
        mov rax, [rdi]
        sub rsp, 8
        call [rax]
        add rsp, 8
        pop rbp
        ret

add:
        add rdi, rsi
        mov rax, rdi
        ret
