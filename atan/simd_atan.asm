; x - x^3/3 + x^5/5 - x^7/7 ...
; float simd_atan(float x, uint32_t n)

; This solution reduces the accuracy of calculations
; because it summarizes negatives with negatives
; and positives with positives infinitely.
; It's wrong from the mathematical point of view because of
; using divergent series.

        global simd_atan

        section .data
denom_init:
        dd 1.0, 3.0, 5.0, 7.0
denom_inc:
        dd 8.0, 8.0, 8.0, 8.0
signs:
        dd 1.0, -1.0, 1.0, -1.0

        section .text

simd_atan:
        mov ecx, [esp + 8]              ; n (assumed to be multiple of 4)

        movss xmm0, [esp + 4]           ; xmm0 = [x, 0, 0, 0]
        shufps xmm0, xmm0, 0x00         ; xmm0 = [x, x, x, x]

        xorps xmm7, xmm7                ; xmm7 = [0, 0, 0, 0]

        movups xmm2, xmm0               ; xmm2 = [x, x, x, x]
        mulps xmm2, xmm2                ; xmm2 = [x^2, x^2, x^2, x^2]

        movups xmm1, xmm0               ; xmm1 = [x, x, x, x]
        movups xmm3, xmm1               ; xmm3 = [x, x, x, x]
        mulps xmm3, xmm2                ; xmm3 = [x^3, x^3, x^3, x^3]
        movups xmm4, xmm3               ; xmm4 = [x^3, x^3, x^3, x^3]
        mulps xmm4, xmm2                ; xmm4 = [x^5, x^5, x^5, x^5]
        movups xmm5, xmm4               ; xmm5 = [x^5, x^5, x^5, x^5]
        mulps xmm5, xmm2                ; xmm5 = [x^7, x^7, x^7, x^7]

        movups xmm6, xmm1               ; xmm6 = [x^1, x^1, x^1, x^1]
        unpcklps xmm6, xmm3             ; xmm6 = [x^1, x^3, x^1, x^3]
        movups xmm1, xmm4               ; xmm1 = [x^5, x^5, x^5, x^5]
        unpcklps xmm1, xmm5             ; xmm1 = [x^5, x^7, x^5, x^7]
        movlhps xmm6, xmm1              ; xmm6 = [x^1, x^3, x^5, x^7]
        movups xmm1, xmm6               ; xmm1 = [x^1, x^3, x^5, x^7]

        movups xmm6, [signs]
        mulps xmm1, xmm6                ; xmm1 = [x^1, -x^3, x^5, -x^7]

        movups xmm3, xmm2               ; xmm3 = [x^2, x^2, x^2, x^2]
        mulps xmm3, xmm3                ; xmm3 = [x^4, x^4, x^4, x^4]
        mulps xmm3, xmm3                ; xmm3 = [x^8, x^8, x^8, x^8]

        movups xmm2, [denom_init]       ; xmm2 = [1, 3, 5, 7]

        movups xmm4, [denom_inc]        ; xmm4 = [8, 8, 8, 8]

        shr ecx, 2                      ; ecx = n / 4

        test ecx, ecx
        jz .done

.loop:
        movups xmm5, xmm1               ; xmm5 = numerators
        divps xmm5, xmm2                ; xmm5 = numerators / denominators
        addps xmm7, xmm5                ; update result

        mulps xmm1, xmm3                ; numerators *= x^8
        addps xmm2, xmm4                ; denominators += 8

        dec ecx
        jnz .loop

.done:
        movups xmm0, xmm7               ; xmm0 = [a, b, c, d]
        movups xmm1, xmm7
        shufps xmm1, xmm1, 0x4E         ; xmm1 = [c, d, a, b]
        addps xmm0, xmm1                ; xmm0 = [a+c, b+d, a+c, b+d]
        movups xmm1, xmm0
        shufps xmm1, xmm1, 0xB1         ; xmm1 = [b+d, a+c, b+d, a+c]
        addps xmm0, xmm1                ; xmm0 = [a+b+c+d, ...] (all lanes have sum)

        movss [esp + 8], xmm0
        fld dword [esp + 8]

        ret

        section .note.GNU-stack
