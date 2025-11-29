        section .text

        global simd_strlen

simd_strlen:
        mov rax, rdi
        vpxor ymm0, ymm0, ymm0

iter:
        vpcmpeqb ymm1, ymm0, [rax]
        vpmovmskb esi, ymm1
        bsf esi, esi
        jnz return
        add rax, 32
        jmp iter

return:
        add rax, rsi
        sub rax, rdi
        vzeroupper
        ret

        section .note.GNU-stack
