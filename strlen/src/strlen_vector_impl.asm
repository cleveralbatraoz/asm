section .text

global _strlen_vector_impl

_strlen_vector_impl:
	mov rax, rdi

iter:
	pxor xmm0, xmm0
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
