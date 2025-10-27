section .text

global call_f

call_f:
	mov edx, [esp + 4]
	mov ecx, [edx]
	push edx
	call [ecx]
	add esp, 4
	ret
