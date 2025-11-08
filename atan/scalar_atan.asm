; x - x^3/3 + x^5/5 - x^7/7 ...
; float scalar_atan(float x, uint32_t n)
; Implementation using SSE scalar single operations (no FPU, no SIMD)

        global scalar_atan

        section .data

one:
        dd 1.0

two:
        dd 2.0

sign:
        dd -1.0

        section .text

scalar_atan:
        mov ecx, [esp + 8]             ; n

        xorps xmm0, xmm0               ; xmm0 = 0.0 (result)

        movss xmm1, [esp + 4]          ; xmm1 = x (current_term)

        movss xmm2, [one]              ; xmm2 = 1.0 (denominator)

        movss xmm3, xmm1               ; xmm3 = x
        mulss xmm3, xmm3               ; xmm3 = x * x

        movss xmm4, [sign]
        mulss xmm3, xmm4               ; xmm3 = -x^2 (multiplier)

        movss xmm5, [two]              ; xmm5 = 2.0

        test ecx, ecx
        jz .done

.loop:
        movss xmm4, xmm1               ; xmm4 = current_term
        divss xmm4, xmm2               ; xmm4 = current_term / denominator
        addss xmm0, xmm4               ; result += current_term / denominator

        mulss xmm1, xmm3               ; current_term *= multiplier

        addss xmm2, xmm5               ; denominator += 2.0

        dec ecx
        jnz .loop

.done:
        movss [esp + 8], xmm0
        fld dword [esp + 8]

        ret

        section .note.GNU-stack
