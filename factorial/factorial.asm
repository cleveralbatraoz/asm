section .text

global main

extern printf

factorial:
	mov ecx, [esp + 4]
	inc ecx
	mov eax, 1

multiply:
	dec ecx
	jz return
	imul eax, ecx
	jmp multiply

return:
	ret

main:
	push 4
	call factorial
	add esp, 4
	push eax
	push msg
	call printf
	add esp, 8
	ret

section .rdata
msg: db "%d", 0
