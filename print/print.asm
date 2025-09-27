        global mod128by10

mod128by10:
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi

; eax = R0
        mov eax, [ebp+8]

; eax += 6*R1
        mov ecx, [ebp+12]
        lea edx, [ecx*4]
        lea edx, [edx + ecx*2]
        add eax, edx

; eax += 6*R2
        mov ecx, [ebp+16]
        lea edx, [ecx*4]
        lea edx, [edx + ecx*2]
        add eax, edx

; eax += 6*R3
        mov ecx, [ebp+20]
        lea edx, [ecx*4]
        lea edx, [edx + ecx*2]
        add eax, edx

; divide eax by 10
        xor edx, edx
        mov ecx, 10
        div ecx                        ; quotient -> eax, remainder -> edx

        mov eax, edx                   ; return remainder

        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
