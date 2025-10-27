section .text

global _strlen_dummy_impl

_strlen_dummy_impl:
	mov rax, rdi

iter:
	cmp byte [rax], 0
	je return
	add rax, 1
	jmp iter

return:
	sub rax, rdi
	ret
