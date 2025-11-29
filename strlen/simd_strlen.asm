        section .text

        global simd_strlen

simd_strlen:
        mov rax, rdi
		pxor xmm0, xmm0

iter:
        pcmpeqb xmm0, [rax]
        pmovmskb esi, xmm0
        bsf esi, esi
		jnz return
        add rax, 16
        jmp iter

return:
        add rax, rsi
        sub rax, rdi
        ret

        section .note.GNU-stack
